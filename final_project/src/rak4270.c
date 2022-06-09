#include "rak4270.h"
#include "string.h"

//get free DMA channel
int dma_c_data;
int dma_c_ctrl;

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


//call back for dma interrupt
static void dma_handler()
{
	
	//clear the interrupt
	dma_hw->ints0 = 1u << dma_c_data;
	//restart the transfer on channel
	// dma_channel_start(dma_c);
}

void setup_dma()
{
	dma_c_data = dma_claim_unused_channel(true);
	//8-bit transfers with write increment after each transfer
	
	// pointing to source and destination respectively
	dma_channel_config dma_c_config = dma_channel_get_default_config(dma_c_data);
	channel_config_set_transfer_data_size(&dma_c_config,DMA_SIZE_8);	//16 bits per transfer inserts too many NULL(\000)s in the buffer
	channel_config_set_dreq(&dma_c_config,DREQ_UART0_RX);	//select DREQ_UART0_RX to get signal from  UART to pace transfers
	channel_config_set_write_increment(&dma_c_config,true);
	channel_config_set_read_increment(&dma_c_config,false);//No read increment since the UART data register is the same address
	channel_config_set_ring(&dma_c_config,true,11);
	dma_channel_configure(
		dma_c_data,
		&dma_c_config,
		response_buffer,	//write buffer
		&uart0_hw->dr,		//Initial read address (UART in this case)
		2048,			//number of transfer
		true				//start immediately
	);
	dma_channel_set_irq0_enabled(dma_c_data,true);
	irq_set_exclusive_handler(DMA_IRQ_0,dma_handler);
	irq_set_enabled(DMA_IRQ_0,true);
}


bool rak4270_init()
{
	char resp[7];	//response buffer
	char exp_resp[]={'O','K',' ','\r','\n','\021','\000'};	//response sent on second try
	char exp_resp2[]={'\000','O','K',' ','\r','\n','\000'};	//response sent on first try. Starts with \000 🤔
	rak4270_send_cmd(LORA_TXFER_MODE);	//transfer mode in loraP2P mode
	rak4270_get_resp(resp);
	if(strcmp(resp,exp_resp) == 0 || strcmp(resp,exp_resp2) == 0)
	{
		setup_dma();
		return true;
	}
	return false;
}

void rak4270_send_cmd(char* cmd_str)
{
	uart_puts(uart_num,cmd_str);
	// uart_write_blocking(uart_num,cmd_str,sizeof(cmd_str));
}



void rak4270_send_cmd_payload(char* cmd_str,char* payload)
{
	char cmd[255];
	char hexStr[128];
	strToHex(payload,hexStr);
	sprintf(cmd,"%s%s%s",cmd_str,hexStr,CR_LF);

	uart_puts(uart_num,cmd);
}


// TODO: setup DMA to get all response back
void rak4270_get_resp(char * reply_str)
{
	while (uart_is_readable(uart_num))
	{
		*reply_str = uart_getc(uart_num);
		reply_str++;
	}
	
	
}

void rak4270_sleep(bool sleep_wake)
{
	if(sleep_wake)
	{
		rak4270_send_cmd(LORA_SLEEP);
	}
	else
	{
		rak4270_send_cmd(LORA_WAKE);
	}
	
}