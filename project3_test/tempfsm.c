/**
    Name: Nubian Tesfai
    Course: SER486
    Project 3

    This class updates the alarm status for the
    sensor endpoint. Updates to the temperature are recorded in
    the update method.
*/

#include "led.h"
#include "alarm.h"
#include "log.h"
#include "tempfsm.h"

static unsigned char current_state;
static unsigned char machine_on;
enum estates{crit_lo = 8, warn_lo1 = 7, warn_lo2 = 6, norm2 = 5, norm1 = 0, norm3 = 1, warn_hi1 = 2, warn_hi2 = 3, crit_hi = 4};

void tempfsm_init() {
    machine_on = 1;
}

void tempfsm_reset() {
    machine_on = 0;
    current_state = norm1;
}

void tempfsm_update(int current, int hicrit, int hiwarn, int locrit, int lowarn) {
    static unsigned char *current_status; //will keep this local variable updated on the device status
    *current_status = machine_on;

    if(*current_status == 0) {
        led_set_blink(" ");
        current_state = norm1;
        return;
    }

    else if(*current_status == 1) {

        //If the current temperature is less than equal low warning parameter
        //If the state is normal 1 or normal 2
        if(current > locrit && current <= lowarn && (current_state == norm1 || current_state == norm2)) {
            led_set_blink("-");
            log_add_record(EVENT_LO_WARN);
            alarm_send(EVENT_LO_WARN);
            current_state = warn_lo1;
        }

        //If the current temperature is greater than equal hi warning parameter
        //If the state is normal 1 or normal 3
        else if(current < hicrit && current >= hiwarn && (current_state == norm1 || current_state == norm3)) {
            led_set_blink("-");
            log_add_record(EVENT_HI_WARN);
            alarm_send(EVENT_HI_WARN);
            current_state = warn_hi1;
        }

        //If current is higher than hi critical parameter
        //The current state must be warning hi 1 or 2
        else if(current >= hicrit && (current_state == warn_hi1 || current_state == warn_hi2)) {
            if(current_state == warn_hi1) {
                led_set_blink(".");
                log_add_record(EVENT_HI_ALARM);
                alarm_send(EVENT_HI_ALARM);
                current_state = crit_hi;
            }
            else if (current_state == warn_hi2){
                led_set_blink(".");//testing if this fixes the hi alarm bug...
                current_state = crit_hi;
            }
        }

        //If current is lower than low critical parameter
        //The current state must be warning low 1 or 2
        else if(current <= locrit && (current_state == warn_lo1 || current_state == warn_lo2)) {
            if(current_state == warn_lo1){
                led_set_blink(".");
                log_add_record(EVENT_LO_ALARM);
                alarm_send(EVENT_LO_ALARM);
            }
            current_state = crit_lo;
        }

        //State transition from critical low to low warning 2
        else if((current > locrit && current_state == crit_lo)) {
            led_set_blink("-");
            log_add_record(EVENT_LO_WARN);
            alarm_send(EVENT_LO_WARN);
            current_state = warn_lo2;
        }

        //State transition from critical hi to hi warning 2
        else if(current < hicrit && current_state == crit_hi) {
            led_set_blink("-");
            //alarm_send(EVENT_HI_WARN);
            current_state = warn_hi2;
        }

        //State transition from warning low 2 to normal 2
        else if(current > lowarn && current_state == warn_lo2) {
            led_set_blink(" ");
            current_state = norm2;
        }

        //State transition from warning hi 2 to normal 3
        else if(current < hiwarn && current_state == warn_hi2) {
            led_set_blink(" ");
            current_state = norm3;
        }
    }
}
