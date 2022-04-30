/*
 * AHT 21 driver
 * v0.00
 * **/
#ifndef _AHT21_H_
#define _AHT21_H_

#include "math.h"


/****define environment as stm32 or rp2040*****/
#define _MY_STM32_DISCO_

#ifdef _MY_STM32_DISCO_
	#include "stm32l4xx_hal_i2c.h"
#else

//get rp2040 i2c library
#endif

/************************/



/*******ADDRESS**************/
#define AHT21_I2C_ADDR (0x38 << 1)

//#define AHT21_INIT_REG_1 0x1B
//#define AHT21_INIT_REG_2 0x1C
//#define AHT21_INIT_REG_3 0x1E


#define AHT21_CMD_CALIBRATE 0xE1

#define AHT21_CMD_MEASURE 0xAC
#define AHT21_CMD_SOFTRESET 0xBA
#define AHT21_CMD_GET_STATUS 0x71
#define AHT21_CMD_INIT 0xBE


#define AHT21_STATUS_INIT_RDY 0x18
#define AHT21_STATUS_BUSY 0x80
#define AHT21_STATUS_CALIBRATED 0x08

#define AHT21_TIME_MEASURE_WAIT 250


/****************************/


typedef struct AHT21
{
	uint16_t addr;				//Sensor I2C address
	uint8_t status;				//Sensor current status
	uint16_t temperature;		//Sensor current calculated temperature
	uint16_t humidity;			//Sensor current calculated humidity
	uint8_t raw_response[6];	//Sensor raw data reading
	uint8_t crc;				//CRC check for raw data sent
#ifdef _MY_STM32_DISCO_
	I2C_HandleTypeDef *hal_i2c;
#else
	//setup i2c handler for rp2040
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
	//TODO: setup i2c handler for rp2040
	//AHT21_STATUS AHT21_init(AHT21 *sensor, uint16_t addr
#endif

//trigger measuring values
AHT21_STATUS AHT21_measure(AHT21 *s);
//read response from sensor
AHT21_STATUS AHT21_read(AHT21 *sensor);

//read response from sensor with CRC
AHT21_STATUS AHT21_read_crc(AHT21 *sensor);


//write data to sensor
AHT21_STATUS AHT21_write(AHT21 *sensor,uint8_t cmd);


//calculate temperature from raw data
AHT21_STATUS AHT21_calculate_actuals(AHT21 *sensor);


//definitions
#ifdef _MY_STM32_DISCO_
	AHT21_STATUS AHT21_init(AHT21 *s, uint16_t addr,I2C_HandleTypeDef *hal_i2c)
	{
		HAL_Delay(100); // 20ms power up
		s->addr = addr;

		uint8_t param[2] = {0x08,0x00};


		HAL_I2C_Mem_Write(hal_i2c, addr, AHT21_CMD_INIT, sizeof(param), param,sizeof(param), 1);
		HAL_Delay(10);
		HAL_I2C_Master_Receive(hal_i2c, s->addr,&s->status, sizeof(s->status), 1);

		if((s->status & AHT21_STATUS_INIT_RDY) != AHT21_STATUS_INIT_RDY)
		{
			HAL_I2C_Mem_Write(hal_i2c, addr, AHT21_CMD_INIT, sizeof(param), param,sizeof(param), 1);
			HAL_Delay(10);
		}
		HAL_Delay(40);
		return AHT21_READY;

	}



	AHT21_STATUS AHT21_measure(AHT21 *s)
	{
		HAL_StatusTypeDef result;
		uint8_t param[2] = {0x33,0x00};

		//Trigger measurement
		HAL_I2C_Master_Transmit(s->hal_i2c, s->addr, AHT21_CMD_MEASURE, sizeof(AHT21_CMD_MEASURE), 5);
		HAL_I2C_Master_Transmit(s->hal_i2c,s->addr,param[0],sizeof(param[0]),5);
		result = HAL_I2C_Master_Transmit(s->hal_i2c,s->addr,param[1],sizeof(param[1]),5);


		HAL_Delay(AHT21_TIME_MEASURE_WAIT);

		if(result == HAL_OK)
		{
			return AHT21_OKAY;
		}


		return AHT21_BUSY;
	}

	AHT21_STATUS AHT21_read(AHT21 *s)
	{
		HAL_StatusTypeDef result;
		AHT21_measure(s);

		do
		{
			HAL_I2C_Master_Receive(s->hal_i2c, s->addr,&s->status, sizeof(s->status), 5);
			HAL_Delay(100);
		}while((s->status & (1 << 7)));

		result = HAL_I2C_Master_Receive(s->hal_i2c, s->addr, s->raw_response, 6, 5);

		if(result == HAL_OK)
		{
			return AHT21_OKAY;
		}
		return AHT21_BUSY;
	}

	float AHT21_readTemp(AHT21 *s)
	{
		AHT21_STATUS aht_res = AHT21_read(s);

		uint8_t half_temp = s->raw_response[3] << 4;

		uint32_t raw_temp = half_temp << 16  | s->raw_response[4] << 8 | s->raw_response[5];
		float temp = 0.0f;


//		if(aht_res == AHT21_OKAY)
//		{
			temp =  ((200 * raw_temp) /( 1 << 20)) - 50;

			temp = 200 * (raw_temp / pow(2,20)) - 50;
//		}
		return temp;
	}

#else
	//setup i2c handler for rp2040
#endif


//
//		HAL_I2C_Master_Receive(sensor->hal_i2c, sensor->addr,&sensor->status, sizeof(sensor->status), 1);
//		while(sensor->status & (1 << 7))
//		{
//			printf(".");
//		}
//
//		HAL_Delay(80);
//		printf("[5]:%x\t[4]:%x\t[3]:%x\t[2]:%x\t[1]:%x\t[0]:%x\r\n",
//				sensor->raw_response[5],sensor->raw_response[4],sensor->raw_response[3],
//				sensor->raw_response[2],sensor->raw_response[1],sensor->raw_response[0]);
//		return AHT21_OKAY;
//	}

#endif //_AHT21_H_
