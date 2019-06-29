/**
    Name: Nubian Tesfai

    Course: SER486

    Instructor: Douglas Sandy, Arizona State University

    This class is for timer1 and is used by the rtc class
    to display the date.
*/

#define TCCR1B (*((volatile unsigned char*)0x81))
#define OCR1AL (*((volatile unsigned char*)0x88))
#define OCR1AH (*((volatile unsigned char*)0x89))
#define TIMSK1 (*((volatile unsigned char*)0x6F))
#define SREG   (*((volatile unsigned char*)0x5F))

#define TC1B_CONFIG 0xC
#define OCIEA 0x2
#define L_VAL 0x23
#define H_VAL 0xF4

//Keeps count of the number of seconds elapsed
static volatile unsigned int count;

/**
    Name: timer1_init

    Parameters: None

    Changes: Register values

    Sets the correct bits in the registers that
    enable CTC mode and set the prescalar, enables
    the output compare match and writes the number
    of ticks to the output compare register.
*/
void timer1_init(void) {
    OCR1AH = H_VAL;
    OCR1AL = L_VAL;
    TIMSK1 = OCIEA;
    TCCR1B = TC1B_CONFIG;
    __builtin_avr_sei();
}

/**
    Name: timer1_get

    Parameters: None

    Changes: Nothing

    Atomic operation that gets the current
    value of count.
*/
unsigned long timer1_get() {
    unsigned char sreg = SREG;
    __builtin_avr_cli();
    unsigned int value = count;
    SREG = sreg;
    return value;
}

/**
    Name: timer1_clear

    Parameters: None

    Changes: clears count variable

    Atomic method that clears the value
    of count.
*/
void timer1_clear(){
    unsigned char sreg = SREG;
    __builtin_avr_cli();
    count = 0;
    SREG = sreg;
}

/**
    Interrupt vector that is executed on
    output compare match with OCR1A
*/
void __vector_11() __attribute__((signal, used, externally_visible));
void __vector_11() {
    count++;
}
