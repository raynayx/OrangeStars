/**
 * 
 *  Type definitions for data to be sent
 * 
 * */

#ifndef _DATA_H_
#define _DATA_H_
#define DATA_ARR_LENGTH 10



typedef struct s_data_t
{
	float temperature;
	float humidity;
} s_data_t;

//data store before being sent
extern s_data_t data_arr[DATA_ARR_LENGTH];

#endif //_DATA_H_