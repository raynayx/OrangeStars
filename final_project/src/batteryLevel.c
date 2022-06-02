
#include "hardware/adc.h"

#include "batteryLevel.h"

#define ADC0_PIN 26
#define ADC0_INPUT 0

void battery_adc_setup()
{
	adc_init();
	adc_gpio_init(ADC0_PIN);
	adc_select_input(ADC0_INPUT);
}



float battery_reading(void)
{
	uint adc_raw = adc_read();

	return (adc_raw*ADC_CONVERT);
}

