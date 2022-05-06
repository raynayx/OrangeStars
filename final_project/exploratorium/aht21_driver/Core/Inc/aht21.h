/*
 * AHT 21 driver
 * v0.00
 * **/
#ifndef _AHT21_H_
#define _AHT21_H_


/****define environment as stm32 or rp2040*****/
#define _MY_STM32_DISCO_

#ifdef _MY_STM32_DISCO_
	#include "stm32l4xx_hal.h"
#else
//get rp2040 i2c library
	#include "hardware/i2c.h"
#endif

/************************/



/*******ADDRESS**************/
#define AHT21_I2C_ADDR (0x38 << 1)

#define AHT21_INIT_REG_1 0x1B
#define AHT21_INIT_REG_2 0x1C
#define AHT21_INIT_REG_3 0x1E


//#define AHT21_CMD_CALIBRATE 0xE1

#define AHT21_CMD_MEASURE 0xAC
#define AHT21_CMD_SOFTRESET 0xBA
#define AHT21_CMD_GET_STATUS 0x71


#define AHT21_CMD_INIT 0xBE


#define AHT21_STATUS_INIT_RDY 0x18
#define AHT21_STATUS_BUSY 0x80
#define AHT21_STATUS_CALIBRATED 0x08

#define AHT21_TIME_MEASURE_WAIT 250

#define AHT21_DENOM 1048576 // 2^20


/****************************/


typedef struct AHT21
{
	uint16_t addr;					//Sensor I2C address
	uint8_t status;					//Sensor current status
	float temperature;				//Sensor current calculated temperature
	float humidity;					//Sensor current calculated humidity
	uint8_t raw_response[6];		//Sensor raw data reading
	uint8_t crc;					//CRC check for raw data sent
#ifdef _MY_STM32_DISCO_
	I2C_HandleTypeDef *hal_i2c; 	//stm32_hal
#else
	i2c_inst_t *hal_i2c;				// rp2040
#endif
}AHT21;


typedef enum AHT21_STATUS
{
	AHT21_OKAY,
	AHT21_READY,
	AHT21_BUSY,
	AHT21_CALIBERATED,
	AHT21_UNCALIBERATED,
	AHT21_INIT_FAILED
}AHT21_STATUS;


//initialise sensor with address

#ifdef _MY_STM32_DISCO_
	AHT21_STATUS AHT21_init(AHT21 *sensor, uint16_t addr,I2C_HandleTypeDef *hal_i2c);
#else
	AHT21_STATUS AHT21_init(AHT21 *sensor, uint16_t addr,i2c_inst_t *i2c);
#endif


//read response from sensor
AHT21_STATUS AHT21_read(AHT21 *sensor);

//read response from sensor with CRC
//AHT21_STATUS AHT21_read_crc(AHT21 *sensor);


//write data to sensor
//AHT21_STATUS AHT21_write(AHT21 *sensor,uint8_t cmd);





#endif //_AHT21_H_
