#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#include "../lib/cli.h"



void toggle(uint8_t pin);

const uint8_t LED = 25U;     // LED connected to GPIO 25


cli c;
char sw[256];

int main() 
{
    initCli(&c,"loraNode");
    c.dev.deviceID = "Tranmit";
    c.dev.deviceSerial = "T-01";
    stdio_init_all();


    gpio_init(LED);  //initialiaze GPIO
    // gpio_init(btn);

  

    gpio_set_dir(LED,true); //set pin to output mode
    // gpio_set_dir(btn,false); //set pin to input mode
    // gpio_set_pulls(btn,true,false);     //enable pull up on btn pin

    // gpio_set_irq_enabled_with_callback(btn,GPIO_IRQ_LEVEL_LOW,true,&toggle);    //setup interrupt on specified GPIO

    showDeviceInfo(&c);
    while(1)
    {


    // printf("GPIO_IN: Address 0x%x with value: 0x%x\n",p,*p);

    // printf("GPIO_OUT Address: 0x%x, value: 0x%x\n",l_state,*l_state);
    // if(on)
    // {
    gpio_put(LED,true);
    sleep_ms(500);
    gpio_put(LED,false);
    sleep_ms(500); 
    // }

    toggle(LED);
    sleep_ms(1000);

    showPrompt(&c);
    scanf("%255s",sw);
    listen(&c,sw);
 
    }
    return 0;
}


void toggle(uint8_t pin)
{
    gpio_put(pin,!gpio_get(pin));
}


//debounce not working yet
// bool debounce ()
// {
//     static uint16_t state = 0;
//     state = (state << 1) | gpio_get(btn) | 0xfe00;
//     return (state == 0xff00);
// }