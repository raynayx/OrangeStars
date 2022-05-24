#include "stdio.h"
#include "pico/stdlib.h"
#include "aht21.h"

const uint8_t LED = 25U;
AHT21 s;

int main()
{
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED, true);

	i2c_init(i2c0, 100 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	sleep_ms(5000);
	while(AHT21_init(&s, AHT21_I2C_ADDR, i2c0) != eAHT21_OKAY)
	{
		printf("AHT21:Init failed\n");
		sleep_ms(5000);
	}

	while (1)
	{
		gpio_put(LED, true);
		sleep_ms(500);
		gpio_put(LED, false);
		sleep_ms(500);

		// float t = AHT21_read_temperature(&s, AHT21_FORCE_READ_DATA);
		// float h = AHT21_read_humidity(&s,AHT21_USE_READ_DATA);
		// printf("Temp:%.2fC\tHum:%.2f\n", t, h);

		_read_status_reg(&s);

		sleep_ms(1000);
		printf("\nStatus: 0x%x\n", s.status);
	}

	return 0;
}