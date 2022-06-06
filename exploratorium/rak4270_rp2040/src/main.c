


#include <stdio.h>
#include "pico/stdlib.h"
#include "rak4270.h"
#include "hardware/adc.h"


#define ADC_RANGE (1<<12)
#define ADC_VREF 3.3
#define ADC_CONVERT (ADC_VREF /(ADC_RANGE - 1))

char resp[512];
const uint8_t LED = 25U; 

int main()
{
	
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED,true);

	adc_init();
	adc_gpio_init(26);
	adc_select_input(0);

	uint adc_raw;
	sleep_ms(4000);
	rak4270_setup();
	rak4270_send_cmd(LORA_TXFER_MODE);

	// uint32_t* UART0_UARTIFLS = (uint32_t*)(UART0_BASE + UART_UARTIFLS_OFFSET);
	// printf("UART_FILS_BITS",UART_UARTIFLS_BITS);
	while(1)
	{
		// *UART0_UARTIFLS = 0x12;

		// printf("UART_UARTIFLS: %32x\n",(uint*)(UART_UARTIFLS_OFFSET + UART0_BASE));
		// printf("Before::: uartfils value at addr:%32x\n",*UART0_UARTIFLS);
		// printf("Before:::uartfils raw addr:%32x\n",UART0_UARTIFLS);
		// // hw_set_bits(UART0_UARTIFLS,(uint32_t)0x10); 	//Interested in bits 3,4,5
		// // printf("After::: uartfils value at addr:%32x\n",*UART0_UARTIFLS);
		// // printf("After::: uartfils raw addr:%32x\n",UART0_UARTIFLS);

		gpio_put(LED,true);
		sleep_ms(500);
		gpio_put(LED,false);
		sleep_ms(500); 

		rak4270_send_cmd(LORA_STATUS);
		printf("%s\n",response_buffer);

		rak4270_send_cmd("at+help\r\n");
		printf("%s\n",response_buffer);

		rak4270_send_cmd("at+version\r\n");
		printf("%s\n",response_buffer);
		// rak4270_send_cmd(LORA_P2P_SEND);
		// sleep_ms(500);
		// rak4270_get_resp(resp);

		// printf(resp);
		// printf("\n");
		// sleep_ms(500);

		// char r[10] = "rene";
		// char voltage[3];

		// adc_raw = adc_read();
		// float volt =  adc_raw*ADC_CONVERT;

		// sprintf(voltage,"%f",volt);
		// printf("%s\n",voltage);
		// rak4270_send_cmd_payload(LORA_P2P_SEND,voltage);

		sleep_ms(1000);
	}
	
	return 0;
}