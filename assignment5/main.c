/*
  main.c

  The main method that calls the functions needed to measure the
  temperature of the atmega328p

  Author: Nubian Tesfai
  Assignment 5
  Spring 2018
 */

#include "uart.h"
#include "delay.h"
#include "led.h"
#include "temp.h"


/* main(void)

   Parameters: none

   Returns: int

   This main function uses the control loop design.
   The uart and temp "Classes" call their respective
   init functions. In the control loop the call to temp_start()
   writes a 1 to the ADSC bit in the ADCSRA register. Once
   the conversion is completed temp_get reads from the ADCL
   ADCH registers and performs the algorithm to convert to
   Celcius.

*/
int main(void)
{

    uart_init();
    led_init();
    temp_init();
    uart_writestr("SER486 HW5 - Nubian Tesfai\n\r");
    while(1) {
        temp_start();
        while(!temp_is_data_ready());
        uart_writedec32(temp_get());
        uart_writestr("\n\r");
        delay(1000);
    }

    return 0;
}
