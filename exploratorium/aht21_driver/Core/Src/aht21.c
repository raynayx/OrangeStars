#include "aht21.h"

//#include <math.h>


//definitions
#ifdef _MY_STM32_DISCO_

AHT21_STATUS AHT21_init(AHT21 *s, uint16_t addr,I2C_HandleTypeDef *hal_i2c)
{
	uint8_t count = 0;
	HAL_Delay(500); // 500ms power up
	s->addr = addr;
	s->hal_i2c = hal_i2c;

	uint8_t param[2] = {0x30,0x00};

	HAL_I2C_Master_Receive(s->hal_i2c, s->addr,&s->status, sizeof(s->status), 1);
	HAL_Delay(10);


	//TODO: Check calibration logic
	while((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY && count == 3)
	{
		HAL_I2C_Master_Transmit(s->hal_i2c, s->addr,param, 2, 5);
		HAL_Delay(10);
		HAL_I2C_Master_Receive(hal_i2c, s->addr,&s->status, sizeof(s->status), 5);
		HAL_Delay(10);
		++count;
	}

	if((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY)
	{
		return AHT21_UNCALIBERATED;
	}
	return AHT21_READY;

}


//trigger measuring values
static AHT21_STATUS _AHT21_measure(AHT21 *s)
{
	HAL_Delay(10);
	HAL_StatusTypeDef result;
	uint8_t param[2] = {0x30,0x00};

	//Trigger measurement
	HAL_I2C_Master_Transmit(s->hal_i2c, s->addr,(uint8_t *) AHT21_CMD_MEASURE, 1, 5);
	result = HAL_I2C_Master_Transmit(s->hal_i2c,s->addr,param,2,5);

	HAL_Delay(AHT21_TIME_MEASURE_WAIT);

	while((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY)
	{
		HAL_I2C_Master_Receive(s->hal_i2c, s->addr,&s->status, sizeof(s->status), 5);
		HAL_Delay(10);
	}

	if(result == HAL_OK)
	{
		return AHT21_OKAY;
	}

	return AHT21_BUSY;
}

//calculate temperature and humidity from raw data
static AHT21_STATUS _AHT21_calculate_actuals(AHT21 *s)
{
	uint8_t half_temp = s->raw_response[3] << 4;
	uint8_t half_hum = s->raw_response[3] >> 4;

	uint32_t raw_temp = half_temp << 16  | s->raw_response[4] << 8 | s->raw_response[5];
	uint32_t raw_hum  = s->raw_response[1] << 16 | s->raw_response[2] << 8 | half_hum;

	s->temperature = ((200 * raw_temp) / AHT21_DENOM) - 50;
	s->humidity = (raw_hum/AHT21_DENOM) * 100;

	return AHT21_OKAY;
}

AHT21_STATUS AHT21_read(AHT21 *s)
{
	HAL_StatusTypeDef result;
	AHT21_STATUS aht_res = _AHT21_measure(s);

	if(aht_res == AHT21_OKAY)
	{
		result = HAL_I2C_Master_Receive(s->hal_i2c, s->addr, s->raw_response, 6, 30);
		HAL_I2C_Master_Receive(s->hal_i2c, s->addr,(uint8_t *) s->crc, 1, 5);
	}

	if(result == HAL_OK)
	{
		_AHT21_calculate_actuals(s);
		return AHT21_OKAY;
	}
	return AHT21_BUSY;
}



#else
AHT21_STATUS AHT21_init(AHT21 *sensor, uint16_t addr,i2c_inst_t *i2c)
{
	uint8_t count = 0;
	sleep_ms(500); // 500ms power up
	s->addr = addr;
	s->hal_i2c = i2c;

	uint8_t param[2] = {0x30,0x00};


	i2c_read_blocking (s->hal_i2c,s->addr,&s->status, sizeof(s->status),false);
	sleep_ms(10);


	//TODO: Check calibration logic
	while((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY && count == 3)
	{
		i2c_write_blocking (s->hal_i2c,s->addr,param,2,false);
		sleep_ms(10);
		i2c_read_blocking (s->hal_i2c,s->addr,&s->status, sizeof(s->status),false);
		sleep_ms(10);
		++count;
	}

	if((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY)
	{
		return AHT21_UNCALIBERATED;
	}
	return AHT21_READY;

}

//trigger measuring values
static AHT21_STATUS _AHT21_measure(AHT21 *s)
{
	sleep_ms(10);

	uint8_t param[2] = {0x33,0x00};

	//Trigger measurement
	i2c_write_blocking (s->hal_i2c,s->addr,(uint8_t *) AHT21_CMD_MEASURE,1,false);
	i2c_write_blocking (s->hal_i2c,s->addr,(uint8_t *) (uint8_t*)param[0],1,false);
	i2c_write_blocking (s->hal_i2c,s->addr,(uint8_t *) (uint8_t*)param[1],1,false);

	sleep_ms(AHT21_TIME_MEASURE_WAIT);

	while((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY)
	{
		i2c_read_blocking (s->hal_i2c,s->addr,&s->status, sizeof(s->status),false);
		sleep_ms(10);
	}

//	if(result == HAL_OK)
//	{
		return AHT21_OKAY;
//	}
//
//	return AHT21_BUSY;
}


//calculate temperature and humidity from raw data
static AHT21_STATUS _AHT21_calculate_actuals(AHT21 *s)
{
	uint8_t half_temp = s->raw_response[3] << 4;
	uint8_t half_hum = s->raw_response[3] >> 4;

	uint32_t raw_temp = half_temp << 16  | s->raw_response[4] << 8 | s->raw_response[5];
	uint32_t raw_hum  = s->raw_response[1] << 16 | s->raw_response[2] << 8 | half_hum;

	s->temperature = ((200 * raw_temp) / AHT21_DENOM) - 50;
	s->humidity = (raw_hum/AHT21_DENOM) * 100;

	return AHT21_OKAY;
}



AHT21_STATUS AHT21_read(AHT21 *s)
{
//	int result;
	AHT21_STATUS aht_res = _AHT21_measure(s);

	if(aht_res == AHT21_OKAY)
	{
		i2c_read_blocking (s->hal_i2c,s->addr,s->raw_response, 6,false);
		i2c_read_blocking (s->hal_i2c,s->addr,(uint8_t *) s->crc, 1,false);
	}

	if(result == HAL_OK)
	{
		_AHT21_calculate_actuals(s);
		return AHT21_OKAY;
	}
	return AHT21_BUSY;
}


#endif

