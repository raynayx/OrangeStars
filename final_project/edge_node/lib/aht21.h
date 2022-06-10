/*
 * AHT 21 driver
 * Port of AHT21 driver written for the Arduino Ecosystem at:
 * https://github.com/Thinary/AHT_Sensor/tree/main/AHT_Sensor
 * 
 * This port works for RP2040 based boards
 * **/
#ifndef _AHT21_H_
#define _AHT21_H_

#include "hardware/i2c.h"

/************************/

/*******ADDRESS**************/
#define AHT21_I2C_ADDR (0x38)

// /*Commnnad registers*/
// #define AHT21_REG_MEASURE 0xAC
// #define AHT21_REG_INIT 0xBE
// #define AHT21_REG_STATUS 0x71
// #define AHT21_REG_SOFTRESET 0xBA

// /*Register controls*/
// /*Init register control*/
// #define AHT21_INIT_CTRL_NORMAL_MODE       0x00  //normal mode on/off       bit[6:5], for AHT1x only
// #define AHT21_INIT_CTRL_CYCLE_MODE        0x20  //cycle mode on/off        bit[6:5], for AHT1x only
// #define AHT21_INIT_CTRL_CMD_MODE          0x40  //command mode  on/off     bit[6:5], for AHT1x only
// #define AHT21_INIT_CTRL_CAL_ON            0x08  //calibration coeff on/off bit[3]
// #define AHT21_INIT_CTRL_NOP               0x00  //NOP control, send after any "AHT1X_INIT_CTRL..."

// /* status byte register controls */
// #define AHT21_STATUS_CTRL_BUSY            0x80  //busy                      bit[7]
// #define AHT21_STATUS_CTRL_NORMAL_MODE     0x00  //normal mode status        bit[6:5], for AHT1x only
// #define AHT21_STATUS_CTRL_CYCLE_MODE      0x20  //cycle mode status         bit[6:5], for AHT1x only
// #define AHT21_STATUS_CTRL_CMD_MODE        0x40  //command mode status       bit[6:5], for AHT1x only
// #define AHT21_STATUS_CTRL_CRC             0x10  //CRC8 status               bit[4], no info in datasheet
// #define AHT21_STATUS_CTRL_CAL_ON          0x08  //calibration coeff status  bit[3]
// #define AHT21_STATUS_CTRL_FIFO_ON         0x04  //FIFO on status            bit[2], no info in datasheet
// #define AHT21_STATUS_CTRL_FIFO_FULL       0x02  //FIFO full status          bit[1], no info in datasheet
// #define AHT21_STATUS_CTRL_FIFO_EMPTY      0x02  //FIFO empty status         bit[1], no info in datasheet

// /* measurement register controls */
// #define AHT21_START_MEASURE_CTRL      0x33  //measurement controls, suspect this is temperature & humidity DAC resolution
// #define AHT21_START_MEASURE_CTRL_NOP  0x00  //NOP control, send after any "AHTXX_START_MEASUREMENT_CTRL..."


// /* sensor delays */
// #define AHT21_CMD_DELAY          10      //delay between commands, in milliseconds
// #define AHT21_MEASUREMENT_DELAY  80      //wait for measurement to complete, in milliseconds
// #define AHT21_POWER_ON_DELAY     250     //wait for AHT2x to initialize after power-on, in milliseconds
// #define AHT21_SOFT_RESET_DELAY   20      //less than 20 milliseconds

// /* misc */
// #define AHT21_I2C_SPEED_100KHZ   100000  //sensor I2C speed 100KHz..400KHz, in Hz
// #define AHT21_I2C_STRETCH_USEC   1000    //I2C stretch time, in usec
// #define AHT21_FORCE_READ_DATA    true    //force to read data via I2C
// #define AHT21_USE_READ_DATA      false   //force to use data from previous read


#define AHT21_DENOM 1048576 // 2^20

#define WATER_VAPOR 17.62F
#define BAROMETRIC_PRESSURE 243.5F

/****************************/

//AHT21 sensor struct
typedef struct AHT21
{
	uint16_t addr;					//Sensor I2C address
	uint8_t status;					//Sensor current status
	float temperature;				//Sensor current calculated temperature
	float humidity;					//Sensor current calculated humidity
	uint8_t raw_resp[6];			//Sensor raw data reading
	uint8_t crc;					//CRC check for raw data sent
	i2c_inst_t *hal_i2c;			// rp2040
}AHT21;


// typedef enum eAHT21_STATUS
// {
// 	eAHT21_OKAY = 0x00,
// 	eAHT21_BUSY,
// 	eAHT21_ACK_ERROR,
// 	eAHT21_DATA_ERROR,
// 	eAHT21_CRC8_ERROR,
// 	eAHT21_ERROR = 0xFF
// }eAHT21_STATUS;


//initialise sensor with address
bool AHT21_init(AHT21 *s,i2c_inst_t *i2c);
float AHT21_get_humidity(AHT21 *s);
float AHT21_get_temperature(AHT21 *s);
float AHT21_get_dew_point(AHT21 *s);
uint8_t AHT21_read_status(AHT21 *s);
void AHT21_reset(AHT21 *s);

#endif //_AHT21_H_