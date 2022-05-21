
#include "stdio.h"
#include "pico/stdlib.h"
// #include "aht21.h"

const uint8_t LED = 25U; 
// AHT21 s;

int main()
{
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED,true);

	// i2c_init(i2c0, 100 * 1000);
	// gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	// gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	// gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	// gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);


	// AHT21_init(&s, AHT21_I2C_ADDR,i2c0);



	while(1)
	{
		gpio_put(LED,true);
		sleep_ms(500);
		gpio_put(LED,false);
		sleep_ms(500);

		// AHT21_read(&s);

		// printf("%f\n",s.temperature);
	}



	return 0;
}