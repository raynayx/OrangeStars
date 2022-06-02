/**
 * 
 * 
*/


#ifndef _BATTERY_LEVEL_H_
#define _BATTERY_LEVEL_H_

#define ADC_RANGE (1<<12)
#define ADC_VREF 3.3
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE -1))


float battery_reading(void);
void battery_adc_setup(void);

#endif	//_BATTERY_H_