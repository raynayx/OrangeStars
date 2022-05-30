#include "rak4270.h"
#include "string.h"


static void strToHex(char *inStr, char *hexStr)
{
	int i, j = 0;
	for (i = 0; i < strlen(inStr); i++)
	{
		sprintf(hexStr + j, "%02X", inStr[i]);
		j += 2;
	}

	hexStr[j] = '\0';
}


void rak4270_setup()
{
	if(uart_is_enabled(uart0))
	{
		printf("RAK4270:: Setup done\n");
		return;
	}
	uart_init(uart0,115200);  //init UART 0
	gpio_set_function(0,GPIO_FUNC_UART); //set GPIO 0 to UART TX
	gpio_set_function(1,GPIO_FUNC_UART); //set GPIO 1 to UART RX
	printf("RAK4270:: Setup done\n");
}

void rak4270_send_cmd(char* cmd_str)
{
	uart_puts(uart0,cmd_str);
	// uart_write_blocking(uart0,cmd_str,sizeof(cmd_str));
}



void rak4270_send_cmd_payload(char* cmd_str,char* payload)
{
	char cmd[255];
	char hexStr[127];
	strToHex(payload,hexStr);
	sprintf(cmd,"%s%s%s",cmd_str,hexStr,CR_LF);

	// printf("%s",cmd);

	uart_puts(uart0,cmd);
}

void rak4270_get_resp(char * reply_str)
{
	while (uart_is_readable(uart0))
	{
		*reply_str = uart_getc(uart0);
		reply_str++;
	}
	
	
}
