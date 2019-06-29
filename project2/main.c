/**
    Author: Nubian Tesfai

    Course: SER486, Arizona State University

    Instructor: Doug Sandy

    This .c file utilizes all the functionality
    introduced throughout the semester. The goal
    is to write valuable config and vpd information
    to the EEPROM on the AVR.
 */
#include "config.h"
#include "delay.h"
#include "eeprom.h"
#include "led.h"
#include "log.h"
#include "uart.h"
#include "rtc.h"
#include "util.h"
#include "vpd.h"

int main(void)
{
    //Delay will init timer0, rtc will init timer1
    uart_init();
    led_init();
    log_init();
    rtc_init();
    vpd_init();
    config_init();
    led_set_blink("--- -.- ");
    rtc_set_by_datestr("01/01/2018 00:00:00");
    uart_writestr("SER486 Project 2 Nubian Tesfai\r\n");
    uart_writestr(vpd.token); //Print values from struct variables
    uart_writestr("\n\r");
    uart_writestr(vpd.manufacturer);
    uart_writestr("\n\r");
    uart_writestr(config.token);
    uart_writestr("\n\r");
    config.use_static_ip = 1;
    log_clear();
    log_add_record(0xaa);
    log_add_record(0xbb);
    log_add_record(0xcc);

    unsigned int dumped = 0;
    while(1){
        led_update();
        if(!eeprom_isbusy()){
            log_update();  //Perform EEPROM dump
            config_update();
        }
        if ((!eeprom_isbusy()) && (!dumped)) {
            dump_eeprom(0, 0x100);
            dumped=1;
        }
    }


    return 0;
}
