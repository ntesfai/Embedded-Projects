/*
  temp.c

  This file implements the functionality needed
  to initialize the atmega328p ADC. The functions
  implemented provide the temperature of the atmega microprocessor

  functions are:

  void init_temp()          -  Initializes the appropriate bits in the ADCSRA & ADMUX

  int temp_data_is_ready()  -  Reads the ADSC bit to see if a conversion is completed

  void temp_start()         -  Begins the conversion from analog input to digital output

  int temp_get()            -  Reads the value from the ADCL ADCH registers and converts
                               the value to a temperature in Celcius
  */

#include "temp.h"
#define ADMUX   (*(volatile char*)0x7C)
#define ADCSRA  (*(volatile char*)0x7A)
#define ADCL    (*(volatile char*)0x78)
#define ADCH    (*(volatile char*)0x79)


/*init_temp()

  parameters: none

  return value: void

  changes: register values

  This method will set the ADCSRA & ADMUX registers on the
  atmega328p such that the ADC is enabled, reference voltage is 1.1v,
  the channel is set to read from the internal temperature sensor and
  the divisor is at 64*/
void temp_init() {
    //Enable the ADC and set the divisor
    ADCSRA |= (1 << 7) | (1 << 2) | (1 << 1);
    //Set the channel to the internal temperature sensor
    //Sets the appropriate reference voltage
    ADMUX |= (1 << 7) | (1 << 6) | (1 << 3);
}


/*temp_is_data_ready()

  parameters: none

  return value: int

  changes: nothing

  This method will check if the ADSC bit in the ADCSRA register is
  high or low. If it is high that means the conversion from analog to
  digital is not yet finished, return 0. Otherwise return 1 if the ADSC
  is low.*/
int temp_is_data_ready(){
    //The and operation checks the sixth bit in the ADCSRA register
    if( (ADCSRA & 1 << 6) )
        return 0;
    else return 1;
}


/*  temp_start()

    parameters: none

    returns: void

    changes: ADSC bit set to 1

    This method sets the ADSC bit in the ADCSRA register.
    This starts the conversion of analog input to digital.
*/
void temp_start() {
    ADCSRA |= 1 << 6;
}


/*  temp_get()

    parameters: none

    returns: int

    changes: nothing

    This method gets the value of a completed
    ADC conversion. First the ADCL register must
    be read. This is important because the ADC will
    never update unless ADCH is read after ADCL
*/
int temp_get() {
    int the_low = ADCL;
    int the_high = ADCH;
    long ret = (the_high << 8) | (the_low);
    return ((ret * 101) / 100) - 273;
}


