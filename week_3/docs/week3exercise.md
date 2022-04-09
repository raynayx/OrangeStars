<!-- On your final project board, make blinky for yourself. Then add a button to turn the LED on and
off. Bonus points for making the button cause an interrupt. Triple bonus points for debouncing
the button signal.
What build environment are you using? You have many free options: STM32CubeMxIDE,
VSCode with Platformio, Platformio, Keil (free version), Arm GNU tools, and so on. For many of
these, there are examples, explore these examples and re-use HALs and code as you can.
Can you step through the code to see what each line does?
Investigate further, using the processor manual:
● What are the hardware registers that cause the LED to turn on and off? (From the
processor manual, don’t worry about initialization.)
● What are the registers that you read in order to find out the state of the button?
● Can you read the register directly and see the button change in a debugger or by
printing out the value of the memory at the register’s address? -->




## Blinky

```C
#include "pico/stdlib.h"
#include "hardware/gpio.h"


const uint8_t LED = 25U;     // LED connected to GPIO 25


int main() 
{
    stdio_init_all();

    gpio_init(LED);

    gpio_set_dir(LED,true);


    while(1)
    {
		gpio_put(LED,true);
		sleep_ms(30);
		gpio_put(LED,false);
		sleep_ms(400); 
    }
    return 0;
}

```

## Blinky with button and interrupt
```C
#include "hardware/gpio.h"
#include "hardware/uart.h"


void toggle();

volatile bool on = false;
volatile int btnCount = 0;

const uint8_t btn = 15U;    // button connected to GPIO 15
const uint8_t LED = 25U;     // LED connected to GPIO 25const 


int main() 
{
    stdio_init_all();

    gpio_init(LED);
    gpio_init(btn);

    gpio_set_dir(LED,true);
    gpio_set_dir(btn,false);
    gpio_set_pulls(btn,true,false);     //enable pull up on btn pin

    gpio_set_irq_enabled_with_callback(btn,GPIO_IRQ_LEVEL_LOW,true,&toggle);    //setup interrupt on specified GPIO

    while(1)
    {

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
```


## Blinky with button debounce
```C

```


## Build environment
- Text editor: VSCode
- GNU ARM 
- Raspberry Pi Pico SDK


## Hardware Registers
- That control the LED gpio
- That gave status of the button gpio
-

## Debugging