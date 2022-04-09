#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

// #include "../lib/cli.h"



void toggle();
bool debounce ();

volatile bool on = false;
volatile int btnCount = 0;

const uint8_t btn = 16U;    // button connected to GPIO 15
const uint8_t LED = 25U;     // LED connected to GPIO 25const 

// cli c;


int main() 
{
    stdio_init_all();

    // initCli(&c, "loraNode");
    
    // c.dev.deviceID = "rpico-1";
    // c.dev.deviceSerial= "000100010-2";

    gpio_init(LED);
    gpio_init(btn);

    gpio_set_dir(LED,true);
    gpio_set_dir(btn,false);
    gpio_set_pulls(btn,true,false);     //enable pull up on btn pin

    gpio_set_irq_enabled_with_callback(btn,GPIO_IRQ_LEVEL_LOW,true,&toggle);    //setup interrupt on specified GPIO
    // showHelp(&c);
    // showPrompt(&c);

    // printf("\t%s> ",c.progname);

    while(1)
    {

//     if(debounce())
//    {
//        toggle();
//    }


    if(on)
    {
        gpio_put(LED,true);
        sleep_ms(30);
        gpio_put(LED,false);
        sleep_ms(400); 
    }
 
    }
    return 0;
}


void toggle()
{
    on = !on;

}

bool debounce ()
{
    static uint16_t state = 0;
    state = (state << 1) | gpio_get(btn) | 0xfe00;
    return (state == 0xff00);
}