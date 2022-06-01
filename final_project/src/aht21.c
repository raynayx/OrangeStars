#include "aht21.h"
#include <stdio.h>

uint8_t sensor_calibrate_cmd[3] = {0xE1,0x08,0x00};
uint8_t sensor_normal_cmd[3] = {0xA8,0x00,0x00};
uint8_t sensor_measure_cmd[3] = {0xAC,0x33,0x00};
uint8_t sensor_reset_cmd = 0xBA;
bool get_r_humidity_cmd = true;
bool get_temperature_cmd = false;


static void _read_sensor(AHT21 *s)
{
	i2c_write_blocking(s->hal_i2c,s->addr,sensor_measure_cmd,3,false);
	sleep_ms(100);

	i2c_read_blocking(s->hal_i2c,s->addr,s->raw_resp,6,true);
	i2c_read_blocking(s->hal_i2c,s->addr,&s->crc,1,false);
}

bool AHT21_init(AHT21 *s,i2c_inst_t *i2c)
{
	s->addr = AHT21_I2C_ADDR;
	s->hal_i2c = i2c;

	i2c_write_blocking(s->hal_i2c,s->addr,sensor_calibrate_cmd,3,false);

	if((AHT21_read_status(s)&0x18) == 0x18)
	{
		return true;
	}
	return false;
}

uint8_t AHT21_read_status(AHT21 *s)
{
	i2c_read_blocking(s->hal_i2c,s->addr,&s->status,1,false);

	return s->status;
}

void AHT21_reset(AHT21 *s)
{
	i2c_write_blocking(s->hal_i2c,s->addr,&sensor_reset_cmd,1,false);
}

float AHT21_get_humidity(AHT21 *s)
{
	uint32_t raw_res;
	_read_sensor(s);
	raw_res = ((s->raw_resp[1] << 16) | (s->raw_resp[2] << 8 )| s->raw_resp[3]) >> 4;
	s->humidity = ((raw_res * 100)/AHT21_DENOM);
	return s->humidity;
}

float AHT21_get_temperature(AHT21 *s)
{
	uint32_t raw_res;
	_read_sensor(s);
	raw_res = (s->raw_resp[3] & 0x0F) << 16 | (s->raw_resp[4] << 8) | s->raw_resp[5];
	s->temperature = (((200 * raw_res)/AHT21_DENOM) - 50);
	return s->temperature;
}