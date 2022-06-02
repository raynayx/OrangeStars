
//pico includes
#include "stdio.h"
#include "pico/stdlib.h"




// project specific includes
#include "aht21.h"
#include "rak4270.h"
#include "console.h"
#include "ws2812.pio.h"
#include "ws2812.h"
#include "batteryLevel.h"

#include "sm.h"

//function prototypes
static void i2c_setup(void);
static void uart_setup(void);
static void button_setup(void);
static void ws2812_setup(void);
static void isr(uint gpio,uint32_t events);




AHT21 s;	//temp sensor object
STATE_T state = STATE_POWER_UP;		//state machine start point
float aht21_temp = 0.0f;		//temperature reading
float aht21_hum = 0.0f;			//humidity reading
float battery_voltage = 0.0f;	//battery voltage
char temp_hum[20];				//data payload
bool debug_mode = false;


#ifdef _BOARDS_SEEED_XIAO_RP2040_H

	uint8_t button = 3u; //interrupt button found on D10
	#define IS_RGBW true
	#define NUM_PIXEL 1
	#define WS2812_PIN 12u
	#define WS2812_PWR 11u

	#define XIAO_I2C_SDA_PIN 28u		//found on A2/D2
	#define XIAO_I2C_SCL_PIN 29u		//found on A3/D3

	// #define XIAO_UART_TX 6
	// #define XIAO_UART_RX 7

#else
	uint8_t button = 22u; //interrupt button
#endif	//seeed_xiao_rp2040

int main()
{
	//setup stdio
	stdio_init_all();

	//seup button
	button_setup();
	// setup i2c bus
	i2c_setup();

	//setup uart
	uart_setup();

	//setup ws2812
	ws2812_setup();

	//setup adc
	battery_adc_setup();

	//wait for serial connection 
	sleep_ms(4000);
	// //setup rak4270
	while(!rak4270_init())
	{
		printf("RAK4270: Init failed\n");
		sleep_ms(2000);
	}
	//setup aht21
	while(!AHT21_init(&s, i2c0))
	{
		printf("AHT21:Init failed\n");
		sleep_ms(2000);
	}

	//setup console
	ConsoleInit();
	while(1)
	{
		if(!debug_mode)
		{
			aht21_temp = AHT21_get_temperature(&s);
			aht21_hum = AHT21_get_humidity(&s);
			battery_voltage = battery_reading();
			sprintf(temp_hum,"%.1fC\t%.1f%%\n",aht21_temp,aht21_hum);
			rak4270_send_cmd_payload(LORA_P2P_SEND,temp_hum);

			printf("%s\t%.1fV\n",temp_hum,battery_voltage);

			sleep_ms(2000);
		}

		if(debug_mode)
		{
			#ifdef _BOARDS_SEEED_XIAO_RP2040_H
				pattern_sparkle(NUM_PIXEL,0);
				sleep_ms(60);
			#endif
			ConsoleProcess();
		}
	}
	return 0;
}



static void i2c_setup(void)
{
	i2c_init(i2c0, 100 * 1000);
	#ifdef _BOARDS_SEEED_XIAO_RP2040_H
		gpio_set_function(XIAO_I2C_SDA_PIN, GPIO_FUNC_I2C);
		gpio_set_function(XIAO_I2C_SCL_PIN, GPIO_FUNC_I2C);
		gpio_pull_up(XIAO_I2C_SDA_PIN);
		gpio_pull_up(XIAO_I2C_SCL_PIN);
	#else
		gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
		gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
		gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
		gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
	#endif
}

static void uart_setup(void)
{	
	#ifdef _BOARDS_SEEED_XIAO_RP2040_H
		uart_init(uart_num,115200);  //init UART 0
		gpio_set_function(0,GPIO_FUNC_UART); //set GPIO 0 to UART TX
		gpio_set_function(1,GPIO_FUNC_UART); //set GPIO 1 to UART RX
	#else
		uart_init(uart_num,115200);  //init UART 0
		gpio_set_function(0,GPIO_FUNC_UART); //set GPIO 0 to UART TX
		gpio_set_function(1,GPIO_FUNC_UART); //set GPIO 1 to UART RX
	#endif
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


static void ws2812_setup(void)
{
	#ifdef _BOARDS_SEEED_XIAO_RP2040_H
		gpio_init(WS2812_PWR);
		gpio_set_dir(WS2812_PWR,true);
		gpio_put(WS2812_PWR,true);		
		PIO pio = pio0;
		int sm = 0;
		uint offset = pio_add_program(pio0,&ws2812_program);
		ws2812_program_init(pio,sm,offset,WS2812_PIN,800000,IS_RGBW);
	#endif

}