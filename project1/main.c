/*
  Author: Nubian Tesfai

  Course: SER486

  Instructor: Douglas Sandy, Arizona State University

  This program implements support for Timer1 and Timer0 of the
  atmega 328p. Timer0 is used as a 1ms periodic tick. Timer1
  as a 1 second periodic tick.
 */

#include "uart.h"
#include "led.h"
#include "rtc.h"
#include "timer1.h"
#include "delay.h"

/*
    Name: main

    Parameters: none

    Main initializes the hardware components and sets delays
    using timers for timer0 and timer1. The number of loops is counted
    when led_update is called and for when it is not.

    In the control loop updates the state of the led, delays for 1 second
    and prints a times stamp over the uart.
*/
int main(void)
{
    led_init();
    uart_init();
    rtc_init();
    led_set_blink("--- -.- ");
    rtc_set_by_datestr("01/01/2018 00:00:00");

    uart_writestr("SER486 Project 1 - Nubian Tesfai\r\n\n");

    //baseline performance
    unsigned long c1 = 0;
    unsigned long c2 = 0;
    delay_set(0, 10000); while(!delay_isdone(0)){c1++; }
    delay_set(0, 10000); while(!delay_isdone(0)){c2++; led_update();}
    uart_writestr("\r\n");
    uart_writedec32(c1); uart_writestr(" ");
    uart_writedec32(c2); uart_writestr("\r\n");

    delay_set(1, 0);
    while(1) {
        led_update();
        if(delay_isdone(0)) {
            delay_set(1, 1000);
            while(!delay_isdone(1));
            uart_writestr(rtc_get_date_string());
            uart_writestr("\r");
        }
    }

    return 0;
}
