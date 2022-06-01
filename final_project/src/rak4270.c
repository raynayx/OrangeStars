#include "rak4270.h"
#include "string.h"

//Convert a string to hex characters
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


bool rak4270_init()
{
	uart_init(uart0,115200);  //init UART 0
	gpio_set_function(0,GPIO_FUNC_UART); //set GPIO 0 to UART TX
	gpio_set_function(1,GPIO_FUNC_UART); //set GPIO 1 to UART RX

	char resp[7];	//response buffer
	char exp_resp[]={'O','K',' ','\r','\n','\021','\000'};	//response sent on second try
	char exp_resp2[]={'\000','O','K',' ','\r','\n','\000'};	//response sent on first try. Starts with \000 🤔
	rak4270_send_cmd(LORA_TXFER_MODE);	//transfer mode in loraP2P mode
	rak4270_get_resp(resp);
	if(strcmp(resp,exp_resp) == 0 || strcmp(resp,exp_resp2) == 0)
	{
		return true;
	}
	return false;
}

void rak4270_send_cmd(char* cmd_str)
{
	uart_puts(uart0,cmd_str);
	// uart_write_blocking(uart0,cmd_str,sizeof(cmd_str));
}



void rak4270_send_cmd_payload(char* cmd_str,char* payload)
{
	char cmd[255];
	char hexStr[128];
	strToHex(payload,hexStr);
	sprintf(cmd,"%s%s%s",cmd_str,hexStr,CR_LF);

	uart_puts(uart0,cmd);
}


// TODO: setup DMA to get all response back
void rak4270_get_resp(char * reply_str)
{
	while (uart_is_readable(uart0))
	{
		*reply_str = uart_getc(uart0);
		reply_str++;
	}
	
	
}
