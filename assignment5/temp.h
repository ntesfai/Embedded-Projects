/**
  temp.h

  Author: Nuian Tesfai
  Course: SER 486
  Assignment 5

  The function declarations for the temp.c file*/

#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED

void temp_init();		/*Initialize the ADMUX & ADCSRA registers to correct values*/
int temp_is_data_ready();	/*Reads the ADSC bit to check if the conversion is finished*/
void temp_start();		/*Starts the conversion from analog input to digital output	*/
int temp_get();			/*Reads the most recent conversion value from ADCL*/

#endif //TEMP_H_INCLUDED
