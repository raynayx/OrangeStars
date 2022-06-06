#include "rak4270.h"
#include "string.h"


char response_buffer[2048];
//get free DMA channel
int dma_c;

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

void dma_handler()
{
	dma_channel_start(dma_c);
	//clear the interrupt
	dma_hw->ints0 = 1u << dma_c;
}

void setup_dma()
{
	dma_c = dma_claim_unused_channel(true);
	//16 bit transfers both read and write increment after each transfer
	// pointing to src and dest respectively
	//select DREQ_UART0_RX
	dma_channel_config dma_c_config = dma_channel_get_default_config(dma_c);
	channel_config_set_transfer_data_size(&dma_c_config,DMA_SIZE_16);
	channel_config_set_dreq(&dma_c_config,DREQ_UART0_RX);
	channel_config_set_write_increment(&dma_c_config,true);
	channel_config_set_read_increment(&dma_c_config,false);

	dma_channel_configure(
		dma_c,
		&dma_c_config,
		&response_buffer,
		&uart0_hw->dr,		//Initial read address (FIFO in this case)
		UINT16_MAX,			//number of transfer 2 bytes each time
		true					//do not start immediately
	);
	dma_channel_set_irq0_enabled(dma_c,true);
	irq_set_exclusive_handler(DMA_IRQ_0,dma_handler);
	irq_set_enabled(DMA_IRQ_0,true);
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

	setup_dma();
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
