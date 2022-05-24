#include "aht21.h"
#include <stdio.h>

//#include <math.h>

static uint8_t _get_busy(AHT21 *s, bool readAHT);
static uint8_t _get_calibration(AHT21 *s);
static bool _set_init_reg(AHT21 *s,uint8_t val);
static bool _checkCRC8(AHT21* s);
static void _read_measurement(AHT21* s);


eAHT21_STATUS AHT21_init(AHT21 *s, uint16_t addr,i2c_inst_t *i2c)
{
	s->addr = addr;
	s->hal_i2c = i2c;
	sleep_ms(AHT21_POWER_ON_DELAY);	//wait for power up
	return AHT21_softreset(s);

}

eAHT21_STATUS AHT21_softreset(AHT21 *s)
{
	_read_status_reg(s);

	int res = i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t*)AHT21_REG_SOFTRESET,1,false);
	
	if(res == PICO_ERROR_GENERIC )
	{
		return eAHT21_ERROR;
	}
	sleep_ms(AHT21_SOFT_RESET_DELAY);

	if((set_normal_mode(s) == true) && (_get_calibration(s) == AHT21_STATUS_CTRL_CAL_ON)); 
	{
		return eAHT21_OKAY;
	}
	return eAHT21_ERROR;

}

bool set_normal_mode(AHT21 *s)
{
	return _set_init_reg(s,AHT21_INIT_CTRL_CAL_ON | AHT21_INIT_CTRL_NORMAL_MODE);
}

static uint8_t _get_calibration(AHT21 *s)
{
	uint8_t val = _read_status_reg(s);
	if (val != eAHT21_ERROR)
	{
		return (val & AHT21_STATUS_CTRL_CAL_ON); 
	}
	return eAHT21_ERROR;
}

static bool _set_init_reg(AHT21 *s,uint8_t val)
{
	sleep_ms(AHT21_CMD_DELAY);
	int res;
	i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t *) AHT21_REG_INIT,1,true);
	i2c_write_blocking(s->hal_i2c,s->addr,&val,1,true);
	res = i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t*)AHT21_INIT_CTRL_NOP,1,false);

	return (res > 0);
}


uint8_t _read_status_reg(AHT21 *s)
{
	

	uint8_t return_val;
	uint8_t cmd = AHT21_REG_STATUS;

	i2c_write_blocking(s->hal_i2c,s->addr,AHT21_REG_STATUS,1,false);
	sleep_ms(AHT21_CMD_DELAY);
	// if(res == PICO_ERROR_GENERIC)
	// {
	// 	return eAHT21_ERROR;
	// }

	if(i2c_get_read_available(s->hal_i2c) == 0)
	{
		return eAHT21_ERROR;
	}

	i2c_read_blocking(s->hal_i2c,s->addr,&return_val,1,false);

	// if(res == PICO_ERROR_GENERIC)
	// {
	// 	return eAHT21_ERROR;
	// }
	return return_val;
}



/**************************************************************************/
/*
    _checkCRC8()

    Check CRC-8-Maxim of AHT2X measured data

    NOTE:
    - part of "readRawMeasurement()" function!!!
    - only AHT2x sensors have CRC
    - initial value=0xFF, polynomial=(x8 + x5 + x4 + 1) ie 0x31 CRC [7:0] = 1+X4+X5+X8
*/
/**************************************************************************/
static bool _checkCRC8(AHT21* s)
{
	uint8_t crc = 0xFF;

	for(uint8_t byteIndex = 0; byteIndex < 6; byteIndex++)
	{
		crc ^= s->raw_response[byteIndex];

		for(uint8_t bitIndex = 8; bitIndex > 0; --bitIndex)
		{
			if(crc & 0x80)
			{
				crc = (crc << 1) ^ 0x31;
			}
			else
			{
				crc = (crc << 1);
			}
		}
	}
	return (crc == s->crc);
}


/**************************************************************************/
/*
    _readMeasurement()

    Start new measurement, read sensor data to buffer & collect errors

    NOTE:
    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only & for
        status description see "_readStatusRegister()" NOTE
*/
/**************************************************************************/
void _read_measurement(AHT21* s)
{
	i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t*)AHT21_REG_MEASURE,1,true);
	i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t*)AHT21_START_MEASURE_CTRL,1,true);
	int res = i2c_write_blocking(s->hal_i2c,s->addr,(uint8_t*)AHT21_START_MEASURE_CTRL_NOP,1,false);
	
	// if(res == PICO_ERROR_GENERIC)
	// {
	// 	s->status = eAHT21_ERROR;
	// 	return;
	// }
	s->status = _get_busy(s,AHT21_FORCE_READ_DATA);
	if(s->status == eAHT21_BUSY)
	{
		sleep_ms(AHT21_MEASUREMENT_DELAY - AHT21_CMD_DELAY);
	}
	else if(s->status != eAHT21_OKAY)
	{
		return;
	}

	res = i2c_get_read_available(s->hal_i2c);

	if(res > 0)
	{
		i2c_read_blocking(s->hal_i2c,s->addr,s->raw_response,6,true);
		i2c_read_blocking(s->hal_i2c,s->addr,&s->crc,1,false);
	}
	else
	{
		s->status = eAHT21_ERROR;
		return;
	}
	s->status = _get_busy(s,AHT21_USE_READ_DATA);

	if(s->status != eAHT21_OKAY)
	{
		return;
	}
	// check on CRC8 calculation
	if(_checkCRC8(s) != true)
	{
		s->status = eAHT21_CRC8_ERROR; 
	}
}

/**************************************************************************/
/*
    _getBusy()

    Read/check busy bit after measurement command

    NOTE:
    - part of "readRawMeasurement()" function!!!
    - 0x80=busy, 0x00=measurement completed, etc
*/
/**************************************************************************/
static uint8_t _get_busy(AHT21 *s, bool readAHT)
{
	if(readAHT)
	{
		sleep_ms(AHT21_CMD_DELAY);

		i2c_read_blocking(s->hal_i2c,s->addr,s->raw_response,1,false);
	}

	if((s->raw_response[0] & AHT21_STATUS_CTRL_BUSY) == AHT21_STATUS_CTRL_BUSY)
	{
		s->status = eAHT21_BUSY;
	}
	else
	{
		return s->status;
	}
}


/**************************************************************************/
/*
    read_temperature()

    Read temperature, in C 

    NOTE:
    - temperature range........ -40C..+85C
    - temperature resolution... 0.01C
    - temperature accuracy..... +-0.3C
    - response time............ 5..30sec
    - measurement with high frequency leads to heating of the
      sensor, must be > 2 seconds apart to keep self-heating below 0.1C

    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only
*/
/**************************************************************************/
float AHT21_read_temperature(AHT21* s,bool readAHT)
{
	if(readAHT == AHT21_FORCE_READ_DATA)
	{
		_read_measurement(s);
	}
	if (s->status != eAHT21_OKAY)
	{
		return eAHT21_ERROR;
	}

	int32_t temp = s->raw_response[3] & 0x0F;
	temp <<= 8;
	temp |= s->raw_response[4];
	temp <<= 8;
	temp |= s->raw_response[5];

	s->temperature = ((float)((temp /AHT21_DENOM) * 200 - 50));
	return s->temperature;
}


/**************************************************************************/
/*
    readHumidity()

    Read relative humidity, in %

    NOTE:
    - relative humidity range........ 0%..100%
    - relative humidity resolution... 0.024%
    - relative humidity accuracy..... +-2%
    - response time............ 5..30sec
    - measurement with high frequency leads to heating of the
      sensor, must be > 2 seconds apart to keep self-heating below 0.1C
    - long-term exposure for 60 hours outside the normal range
      (humidity > 80%) can lead to a temporary drift of the
      signal +3%, sensor slowly returns to the calibrated state at normal
      operating conditions

    - sensors data structure:
      - {status, RH, RH, RH+T, T, T, CRC*}, *CRC for AHT2x only

    - normal operating range T -20C..+60C, RH 10%..80%
    - maximum operating rage T -40C..+80C, RH 0%..100%
*/
/**************************************************************************/
float AHT21_read_humidity(AHT21 *s,bool readAHT)
{
	if(readAHT == AHT21_FORCE_READ_DATA)
	{
		_read_measurement(s);
	}
	if(s->status != eAHT21_OKAY)
	{
		return eAHT21_ERROR;
	}

	uint32_t humidity = s->raw_response[1];
	humidity <<= 8;
	humidity |= s->raw_response[2];
	humidity <<= 4;
	humidity |= s->raw_response[3] >> 4;

	if(humidity > 0x100000)
	{
		humidity = 0x100000;
	}

	s->humidity = ((float) (humidity/0x100000) * 100);

	return s->humidity;
}