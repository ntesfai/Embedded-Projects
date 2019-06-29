/**
Author: Nubian Tesfai
Course: SER 486
Lab # = 1
 */

//Define the needed registers on the atmega microprocessor
#define DDRB (*((volatile char*)0x24))
#define PINB (*((volatile char*)0x23))
#define PORTB (*((volatile char*)0x25))
#define F_CPU 16000000UL
#include "uart.h"

//function prototype
void delay();

int main(void) {

    //Initialize the 1st bit for output
    DDRB |= 0x02;
    PORTB = 0x02;
    uart_writehex8(PORTB & 1 << 1);
    while(1){
        //delay();
        //PINB = 0x02;    //this will toggle the bit referenced in PINB
    }

    return 0;
}

/*Nested for-loop implementation of a delay function
  Compiler optimizations MUST BE OFF*/

void delay() {
    unsigned int i;
    unsigned int j;
    for(i = 0; i < 1200; i++)
        for(j = 0; j < 1270; j++)
        {}
}
