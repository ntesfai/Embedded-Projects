/**
    Name: Nubian Tesfai
    Course: SER486
    Project 3

    This class provides functionality for
    the packet command parser. Updating the
    values for tcrit_lo, twarn_lo, tcrit_hi & twarn_hi
*/
#include "util.h"
#include "config.h"


static char uninitialized = 0;
static int tcrit_lo, tcrit_hi, twarn_hi, twarn_lo;

/**
    Name: update_tcrit_hi
    Parameters: int
    Changes: This method updates the max temperature.
    The value of the parameter must be greater than twarn_hi.
*/
int update_tcrit_hi(int temp_value) {

    if(!uninitialized) {
        tcrit_lo = -40;
        tcrit_hi = 150;
        twarn_hi = 50;
        twarn_lo = 10;

        uninitialized++;
    }
    if(temp_value > twarn_hi) {
        tcrit_hi = temp_value;
        config.hi_alarm = tcrit_hi;
        return 0;
    }
    else
        return 1;
}

/**
    Name: update_tcrit_lo
    Parameters: int
    Changes: This method updates the tcrit_lo value.
    The value of the parameter must be less than twarn_lo
*/int update_tcrit_lo(int temp_value){

    if(!uninitialized) {
        tcrit_lo = -40;
        tcrit_hi = 150;
        twarn_hi = 50;
        twarn_lo = 10;

        uninitialized++;
    }
    if(temp_value < twarn_lo){
        tcrit_lo = temp_value;
        config.lo_alarm = tcrit_lo;
        return 0;
    }
    else
        return 1;
}

/**
    Name: update_twarn_hi
    Parameters: int
    Changes: This method updates the max warning temperature.
    The value of the parameter must be greater than twarn_lo
    and less than tcrit_hi.
*/
int update_twarn_hi(int temp_value) {

    if(!uninitialized) {
        tcrit_lo = -40;
        tcrit_hi = 150;
        twarn_hi = 50;
        twarn_lo = 10;

        uninitialized++;
    }

    if(temp_value < tcrit_hi && temp_value > twarn_lo){
        twarn_hi = temp_value;
        config.hi_warn = twarn_hi;
        return 0;
    }
    else
        return 1;
}

/**
    Name: update_twarn_lo
    Parameters: int
    Changes: This method updates the low warning temperature.
    The value of the parameter must be greater than tcrit_lo
    and less than twarn_hi.
*/
int update_twarn_lo(int temp_value){

    if(!uninitialized) {
        tcrit_lo = -40;
        tcrit_hi = 150;
        twarn_hi = 50;
        twarn_lo = 10;

        uninitialized++;
    }
    if(temp_value > tcrit_lo && temp_value < twarn_hi){
        twarn_lo = temp_value;
        config.lo_warn = twarn_lo;
        return 0;
    }
    else
        return 1;
}
