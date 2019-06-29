/**
    Author: Nubian Tesfai
    Course: SER486
    Instructor: Douglas Sandy, Arizona State University

    This program handles the task of outputting a message
    via LED in Morse Code. The LED will stay lit or off
    based on the amount of time passed to the timer.
*/

#include "delay.h"
#include "led.h"
//#include "uart.h"
#define PORTB (*((volatile char*)0x25))
#define DDRB (*((volatile char*)0x24))

char* blink_msg;
static unsigned int blink_pos = 0;
static unsigned char blink_state;

enum {PHASE1 = 1, PHASE2 = 2};

/**
    Name: led_set_blink

    Parameters: char*

    Changes: the message, blink position, blink state

    This method sets a new value to blink_msg and clears the
    value in blink_pos and sets blink_state to PHASE1.
*/

void led_set_blink(char* msg) {
    blink_msg = msg;
    blink_pos = 0;
    blink_state = PHASE1;
    delay_set(0, 0); led_off();
}

/**
    Name: led_update

    Parameters: none

    Changes: The state of the led and FSM

    This method handles the task of updating the FSM
    state as well as turning on and off the led
    based on the message in blink_msg for a set
    amount of time.
*/
void led_update() {
    //If the message is null
    if(blink_msg == 0){
        return;
    }
    //If the delay is finished
    if(!delay_isdone(0))
        return;

    if(blink_state == PHASE1) {
        if(blink_msg[blink_pos] == '.') {
            led_on();
            //uart_writechar('.');
            delay_set(0, 250);
            while(!delay_isdone(0));
            //led_off();
            blink_state = PHASE2;
            return;
        }
        else if(blink_msg[blink_pos] == '-') {
            led_on();
            //uart_writechar('-');
            delay_set(0, 750);
            while(!delay_isdone(0));
            //led_off();
            blink_state = PHASE2;
            return;
        }
        else if(blink_msg[blink_pos] == ' ') {
            led_off();
            //uart_writechar(' ');
            delay_set(0, 1000);
            while(!delay_isdone(0));
            blink_pos++;
            return;
        }
        else if(blink_msg[blink_pos] != 0) {
            //uart_writestr(" other ");
            blink_pos++;
            return;
        }
        else{
            blink_pos = 0;
            return;
        }
    }

    else if(blink_state == PHASE2){
        led_off();
        delay_set(0, 100);
        while(!delay_isdone(0));
        blink_state = PHASE1;
        blink_pos++;
        return;
    }
}
