#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "console.h"

int main() {
	stdio_init_all(); // UART setup for both input and output
	gpio_init(25);
	gpio_set_dir(25,true);

	sleep_ms(5000);
	ConsoleInit(); 

	while(1) 
	{
		ConsoleProcess();
		sleep_ms(2); 
	}	
}