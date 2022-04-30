#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "console.h"





int main()
{

	stdio_init_all();

	
	sleep_ms(10000);

	// ConsoleInit();

	char c;

	while(1)
	{
		// ConsoleProcess();

		c = getchar();
		uart_putc(uart0,c);
	}
	return 0;
}