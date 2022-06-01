
//pico includes
#include "stdio.h"
#include "pico/stdlib.h"



// project specific includes
#include "aht21.h"
#include "rak4270.h"
#include "console.h"
#include "sm.h"

//function prototypes
static void i2c_setup(void);
static void button_setup(void);
static void isr(uint gpio,uint32_t events);




AHT21 s;	//temp sensor object
STATE_T state = STATE_POWER_UP;		//state machine start point
float aht21_temp = 0.0f;		//temperature reading
float aht21_hum = 0.0f;			//humidity reading
char temp_hum[20];				//data payload
bool debug_mode = false;

uint8_t button = 22u;	//interrupt button

int main()
{
	//setup stdio
	stdio_init_all();

	//seup button
	button_setup();
	// setup i2c bus
	i2c_setup();

	// //setup rak4270
	while(!rak4270_init())
	{
		printf("RAK4270: Init failed\n");
		sleep_ms(5000);
	}
	//setup aht21
	while(!AHT21_init(&s, i2c0))
	{
		printf("AHT21:Init failed\n");
		sleep_ms(5000);
	}

	sleep_ms(4000);
	//setup console
	ConsoleInit();
	while(1)
	{
		if(!debug_mode)
		{
			aht21_temp = AHT21_get_temperature(&s);
			aht21_hum = AHT21_get_humidity(&s);
			sprintf(temp_hum,"%.1fC\t%.1f%%\n",aht21_temp,aht21_hum);
			rak4270_send_cmd_payload(LORA_P2P_SEND,temp_hum);

			sleep_ms(5000);
		}


		if(debug_mode)
		{
			ConsoleProcess();
		}
	}
	return 0;
}



static void i2c_setup(void)
{
	i2c_init(i2c0, 100 * 1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

static void button_setup(void)
{
	gpio_init(button);
	gpio_set_dir(button,false);
	gpio_pull_up(button);
	
	gpio_set_irq_enabled_with_callback(button,GPIO_IRQ_EDGE_FALL,true,&isr);
}

static void isr(uint gpio,uint32_t events)
{
	//respond to interrupt from specific pin
	if(gpio == button)
	{
		debug_mode = !debug_mode;
	}
	
}