// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.

#include "consoleIo.h"
#include <stdio.h>

eConsoleError ConsoleIoInit(void)
{
	return CONSOLE_SUCCESS;
}

// This is modified for the Wokwi RPi Pico simulator. It works fine 
// but that's partially because the serial terminal sends all of the 
// characters at a time without losing any of them. What if this function
// wasn't called fast enough?
// eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
// {
// 	uint32_t i = 0;
// 	// char ch;

// 	// while (uart_is_readable(uart0)) 
// 	// {
//   	// ch = uart_getc(uart0);

// 	//   while((ch != 0x0A) && (i <bufferLength))
// 	//   {
// 	// 	  uart_putc(uart0, ch); // echo
// 	// 	buffer[i] = (uint8_t) ch;
// 	// 	ch = uart_getc(uart0);
// 	// 	i++;
// 	//   }
//   	uart_read_blocking (uart0,buffer, bufferLength);
// 	// }

// 	// *readLength = i;
// 	return CONSOLE_SUCCESS;
// }


eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
	uint32_t i = 0;
	char ch;

	ch = getchar();

	while((ch != EOF) && (i< bufferLength))
	{
		buffer[i] = (uint8_t) ch;
		i++;
		ch = getchar();
	}
	*readLength = i;

	// scanf("%s",buffer);
	return CONSOLE_SUCCESS;
}


eConsoleError ConsoleIoSendString(const char *buffer)
{
	printf("%s", buffer);
	return CONSOLE_SUCCESS;
}

