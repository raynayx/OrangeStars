
#include "stdio.h"
#include "pico/stdlib.h"
#include "aht21.h"

const uint8_t LED = 25U; 
AHT21 s;

int main()
{
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED,true);

	i2c_init(i2c0, 100 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	sleep_ms(5000);
	eAHT21_STATUS r = AHT21_init(&s, AHT21_I2C_ADDR,i2c0);

	printf("status: 0x%x",r);

	while(1)
	{
		gpio_put(LED,true);
		sleep_ms(500);
		gpio_put(LED,false);
		sleep_ms(500);

	
		printf("Temp:%.2fC\tHum:%.2f\n",
		AHT21_read_temperature(&s,AHT21_FORCE_READ_DATA),
		AHT21_read_humidity(&s,AHT21_USE_READ_DATA));
	}



	return 0;
}