


#include <stdio.h>
#include "pico/stdlib.h"
#include "rak4270.h"

char resp[512];
const uint8_t LED = 25U; 

int main()
{
	sleep_ms(10000);
	stdio_init_all();
	gpio_init(LED);
	gpio_set_dir(LED,true);
	
	rak4270_setup();
	rak4270_send_cmd(LORA_TXFER_MODE);
	while(1)
	{
		gpio_put(LED,true);
        sleep_ms(500);
        gpio_put(LED,false);
        sleep_ms(500); 

		rak4270_send_cmd(LORA_P2P_SEND);
		sleep_ms(500);
		rak4270_get_resp(resp);

		printf(resp);
		printf("\n");
		sleep_ms(500);
	}
	
	return 0;
}