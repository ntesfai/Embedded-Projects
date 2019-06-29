/**
    Name: Nubian Tesfai
    Course: SER486
    Project 3

    This class handles the initialization
    and other procedures for the avr watch dog timer.
*/
#include "wdt.h"
#include "led.h"
#include "log.h"
#include "config.h"

#define WDTCSR (*((unsigned volatile char*)0x60))
#define MCUSR  (*((unsigned volatile char*)0x54))

//Bits
#define WDIE 0x40
#define WDIE_CLR_BIT 6
#define WDCE_CONFIG 0x18
#define WDSCALE 0xF
#define WDE 0x8

static volatile unsigned int counting_down;

//

void wdt_init() {
    __builtin_avr_cli();
    __builtin_avr_wdr();
    MCUSR = 0;
    WDTCSR = 0;
    WDTCSR |= WDCE_CONFIG;
    WDTCSR = (WDSCALE) | ( WDIE) | (WDE);
  //four clock cycles to set WDE and pre-scalar bits
    __builtin_avr_sei();

}

void wdt_reset() {
    __builtin_avr_wdr();
    counting_down = 0;
}

//WDIE bit in WDTCSR must be written with a low value
//WDIE must be set AFTER each interrupt to prevent a System reset (see init)
void wdt_force_restart() {
    WDTCSR &= ~(1 << WDIE_CLR_BIT);
    while(1);
}

void __vector_6() __attribute__((signal, used, externally_visible));
void __vector_6(){
    counting_down = 1;
    led_on(); //testing the isr
    log_add_record(EVENT_WDT);
    log_update_noisr();
    config_update_noisr();
    while(1);
}
