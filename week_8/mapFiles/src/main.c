


#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "rak4270.h"
#include "hardware/adc.h"


#define ADC_RANGE (1<<12)
#define ADC_VREF 3.3
#define ADC_CONVERT (ADC_VREF /(ADC_RANGE - 1))

char resp[512];
const uint8_t LED = 25U; 


extern uint __bss_start__;
extern uint __bss_end__;
extern uint __HeapLimit;
extern uint __end__;
extern uint __stack;
extern uint __StackLimit;
extern uint __data_start__;
extern uint __data_end__;

// extern int __my_test_var__;


uint32_t* pVar;
volatile uint32_t StackPointer;		//pointer to Stack


int gInit = 25;
int gUnit;

void staticVar()
{
	static int staticVarInFun = 0;
	++staticVarInFun;

	printf("Static variable in function:\t0x%p\n",&staticVarInFun);
	
	__asm__ volatile ("mov %0, r13\n\t" : "=r"(StackPointer));		// NL and TAB used to separate instructions
	printf("Stack pointer:\t0x%X\n",StackPointer);
}

void var()
{
	int varInFun = 0;
	++varInFun;
	printf("Non-static Variable in function:\t0x%p\n", &varInFun);

	__asm__ volatile ("mov %0, r13;" : "=r"(StackPointer));
	printf("Stack pointer:\t0x%X\n",StackPointer);
}

int main()
{
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED,true);
	adc_init();
	adc_gpio_init(26);
	adc_select_input(0);

	sleep_ms(10000);		//wait a bit in order not miss the serial print out


	uint adc_raw;
	
	rak4270_setup();
	rak4270_send_cmd(LORA_TXFER_MODE);

	pVar = (uint32_t*)&gInit;
	printf("\n\nInitialized global variable:\t0x%p\n",pVar);
	
	pVar = (uint32_t*) &gUnit;
	printf("Uninitialized global variable:\t0x%p\n",pVar);

	staticVar();
	printf("\n");
	var();


	printf("\n.data start:\t0x%p\n",&__data_start__);
	printf(".data end:\t\t0x%p\n",&__data_end__);
	printf(".data length:\t0x%X bytes\n",(&__data_end__ - &__data_start__));

	printf("\n");
	printf(".bss start:\t\t0x%p\n",&__bss_start__);
	printf(".bss end:\t\t0x%p\n",&__bss_end__);
	printf(".bss length:\t0x%X bytes\n",(&__bss_end__ - &__bss_start__));

	printf("\n");

	printf("Heap start:\t\t0x%p\n",&__end__);	//starts after .bss ends and grows from low to high memory
	printf("Heap end:\t\t0x%p\n",&__HeapLimit);	//end of Heap
	printf("Heap length:\t0x%X bytes\n",(&__HeapLimit - &__end__));

	printf("\n");

	printf("Stack start:\t0x%p\n",&__stack);	//Stack grows from the highest memory location to the lowest
	printf("Stack end:\t\t0x%p\n",&__StackLimit);			//Stack is full at lowest memory after heap ends
	printf("Stack length:\t0x%X bytes\n",(&__stack - &__StackLimit));

	printf("\n");
	__asm__ volatile ("mov %0, r13;" : "=r"(StackPointer));
	printf("Stack pointer:\t0x%X\n",StackPointer);

	uint32_t* pHeap = malloc(1);
	printf("Heap pointer:\t0x%p\n",pHeap);
	
	// printf("Address of my test var in .ld\t%p\n",&__my_test_var__);
	printf("\n");
	while(1)
	{
		gpio_put(LED,true);
		sleep_ms(500);
		gpio_put(LED,false);
		sleep_ms(500); 

		// rak4270_send_cmd(LORA_P2P_SEND);
		// sleep_ms(500);
		// rak4270_get_resp(resp);

		// printf(resp);
		// printf("\n");
		// sleep_ms(500);

		char voltage[3];

		adc_raw = adc_read();
		float volt =  adc_raw*ADC_CONVERT;

		sprintf(voltage,"%f",volt);
		// printf("%s\n",voltage);
		rak4270_send_cmd_payload(LORA_P2P_SEND,voltage);

		sleep_ms(1000);
	}
	
	return 0;
}