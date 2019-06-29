/*SER486 Assignment 4
  Author: Nubian Tesfai
  Spring 2018
 */
#define DDRB (*((volatile char*)0x24))
#define PINB (*((volatile char*)0x23))
#include "output.h"
#include "hw4lib.h"

/*Main method with control loop design that blinks an led*/
int main(void)
{
    //Set pin 1 on port B for output
    DDRB |= 0x02;
    //initialize the uart
    uart_init();
    //write the course and your name
    writestr("SER486 HW4 - Nubian Tesfai\n\r");
    //output a number to the uart
    writehex8(0x0A);
    writestr("\n\r");
    //output another number using writehex16
    writehex16(0xC0DE);
    writestr("\n\r");
    while(1){
        //loop forever while blinking the led
        blink_led("--- -.-");  //"Ok in morse code"
    }


    return 0;
}
