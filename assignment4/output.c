/********************************************************
 * output.c
 *
 * SER486 Assignment 4
 * Spring 2018
 * Written By:  Doug Sandy (instructor)
 * Modified By:
 *
 * this file implements functions associated with SER486
 * homework assignment 4.  The procedures implemented
 * provide led and console output support for debugging
 * of future SER 486 assignments.
 *
 * functions are:
 *    writestr(char *)  - a function to write an ascii
 *                        null-terminated string to the
 *                        avr atmega 328P uart port.
 *                        (instructor provided)
 *
 *    writehex8(unsigned char) - a function to write the
 *                        hexadecimal representation of
 *                        an 8-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    writehex16(unsigned int) - a function to write the
 *                        hexadecimal representation of
 *                        a 16-bit unsigned integer to
 *                        avr atmega 328P uart port.
 *
 *    blink_led(char *) - a function to send a specific
 *                        blink pattern to the development
 *                        board's user-programmable LED.
 *
 *    delay(unsigned int) - delay code execution for
 *                        the specified amount of milliseconds.
 *                        (instructor provided)
 */

 #include "hw4lib.h"

 /* macro definitions used by delay() */
 #define MSEC_PER_SEC     1000
 #define CLOCKS_PER_LOOP  16
 #define LOOPS_PER_MSEC   ((F_CPU/MSEC_PER_SEC)/CLOCKS_PER_LOOP)

/**********************************
 * delay(unsigned int msec)
 *
 * This code delays execution for for
 * the specified amount of milliseconds.
 *
 * arguments:
 *   msec - the amount of milliseconds to delay
 *
 * returns:
 *   nothing
 *
 * changes:
 *   nothing
 *
 * NOTE: this is not ideal coding practice since the
 * amount of time spent in this delay may change with
 * future hardware changes.  Ideally a timer should be
 * used instead of a set of for loops.  Timers will be
 * taught later in the semester.
 */
void delay(unsigned int msec)
{
    volatile unsigned i,j ;  /* volatile prevents loops from being optimized away */

    /* outer loop, loops for specified number of milliseconds */
    for (i=0; i<msec; i++) {
        /* inner loop, loops for 1 millisecond delay */
        for (j=0;j<LOOPS_PER_MSEC;j++) {}
    }
}


/**********************************
 * writestr(char * str)
 *
 * This code writes a null-terminated string
 * to the UART.
 *
 * arguments:
 *   str - a pointer to the beginning of the
 *         string to be printed.
 *
 * returns:
 *   nothing
 *
 * changes:
 *   the state of the uart transmit buffer will
 *   be changed by this function.
 *
 * NOTE: uart_init() should be called this function
 *   is invoked for the first time or unpredictable
 *   UART behavior may result.
 */
void writestr(char * str)
{
    unsigned int i;

    /* loop for each character in the string */
    for (i=0; str[i]!=0;i++) {
        /* output the character to the UART */
        uart_writechar(str[i]);
    }
}

/************************************************************
* STUDENT CODE BELOW THIS POINT
*************************************************************/
/**
 Nubian Tesfai
 SER 486 Assignment 4
 Spring 2018*/

/*An unsigned char gets converted into its ASCII equivalent (base 16)
simply get each nibble from the byte, this will be the index
to the hex array*/
void writehex8(unsigned char num)
{
    unsigned char rslt[2];
    unsigned char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9' , 'A', 'B', 'C', 'D', 'E', 'F'};
    /*Loop over the rslt string assigning the value
      corresponding to the index in the hex array*/
    unsigned char i;
    for(i = 0; i < 2; i++) {
        unsigned char nibble;
        nibble = num;
        num >>= 4; //Get the next nibble
        rslt[i] = hex[nibble & 0x0F]; //mask the upper four bits
    }
    /*Now the array needs to be reversed since
    we started from the lsb*/
    unsigned char temp = rslt[1];
    rslt[1] = rslt[0];
    rslt[0] = temp;
    //iterate over the string and output each character
    for(i = 0; i < 2; i++)
        uart_writechar(rslt[i]);
}

/*The unsigned int is coverted to its ASCII equivalent (base 16)
simply get each nibble from the bytes, this will be the index
into the hex array*/
void writehex16(unsigned int num)
{
    //unsigned char len = 3;
    unsigned char rslt[4];
    unsigned char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9' , 'A', 'B', 'C', 'D', 'E', 'F'};
    /*Loop over the rslt string assigning the value
    corresponding to the index in the hex array*/
    unsigned char i;
    for(i = 0; i < 4; i++) {
        unsigned char nibble;
        nibble = num;
        num >>= 4;
        rslt[i] = hex[nibble & 0x000F];    //mask the upper twelve bits
    }
    /*Now the array must be reversed since we started
    from the lsb*/
    for(i = 0; i < 2; i++){
        rslt[i] ^= rslt[4-i-1];
        rslt[4-i-1] ^= rslt[i];
        rslt[i] ^= rslt[4-i-1];

    }
    //iterate over the string and print each character
    for(i = 0; i < 4; i++)
        uart_writechar(rslt[i]);
}

//pre processor directive for PINB
#define PINB (*((volatile char*)0x23))
/*Implementation of the blink function that blinks the LED
on the arduino*/
void blink_led(char *msg)
{
    //iterate over the string until
    //check for any - or . or space characters then increment i
    unsigned int i = 0;
    while(*(msg + i)) {
        if(*(msg + i) == '-'){
            PINB = 0x02;
            delay(750);
            PINB = 0x02;
            delay(250);
        }
        else if(*(msg + i) == '.'){
            PINB = 0x02;
            delay(250);
            PINB = 0x02;
            delay(100);
        }
        else if(*(msg + i) == ' ') {
            delay(1000);
        }
        i++;
    }
}
