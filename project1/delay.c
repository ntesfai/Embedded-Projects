/**
    Author: Nubian Tesfai

    Course: SER486

    Instructor: Douglas Sandy, Arizona State University

    This "class" implements the functionality for the
    timer0. There are two instances of the delay "class"
    where the count for each instance is maintained in an
    array (count).
*/

#define TCCR0A (*((volatile unsigned char*)0x44))
#define TCCR0B (*((volatile unsigned char*)0x45))
#define TIMSK0 (*((volatile unsigned char*)0x6E))
#define OCR0A (*((volatile unsigned char*)0x47))
#define SREG (*((volatile unsigned char*)0x5F))

#define TIMERA_CONFIG 0x2
#define OCIEA 0x2
#define TIMERB_CONFIG 0x3
//#define TICKS 250

static unsigned volatile int count[2] = {0, 0};
unsigned int limit[2];
unsigned const char TICKS = 249;

/** name: init

   parameters: none

   changes: sets the TCCR0A register bits for CTC mode

   and sets the prescalar to 64, as well as enables the
   OCIE0A bit which sets the corresponding bit in the status
   register.
*/
void init() {
    TIMSK0 = OCIEA;          //Enable interrupts on OCR0A on compare reg A
    TCCR0A = TIMERA_CONFIG;  //Set to CTC mode
    TCCR0B = TIMERB_CONFIG;  //Set prescalar to 64
    OCR0A = TICKS;
    __builtin_avr_sei();
}

/**
    name: get

    parameters: unsigned int num

    changes: nothing

    Retrieves the specified value for count instance.
    Must disable global interrupts to ensure atomic operation.
    Copy then restore the value of SREG once count value is read.
*/
unsigned delay_get(unsigned num) {
    unsigned char sreg_state = SREG;
    __builtin_avr_cli();
    unsigned int value = count[num];
    SREG = sreg_state;
    return value;
}

/**
   name: set

   parameters: unsigned int num

   changes: nothing

   Checks that the init() function was called. Gets the state of SREG
   and clears the count variable for a specific instance and sets the time
   limit. SREG is then assigned its original value.
*/
void delay_set(unsigned int num, unsigned int time) {
    static unsigned char initialized = 0;
    if(!initialized) {
        init();
        initialized = 1;
    }
    unsigned const char sreg_state = SREG;
    __builtin_avr_cli();
    count[num] = 0;
    limit[num] = time;
    SREG = sreg_state;
}

/**
    name: is_done

    parameters: int num

    changes: nothing

    Returns 1 if the particular delay instance
    count value is equal to the limit. Otherwise
    return 0.
*/
unsigned delay_isdone(unsigned int num) {
    if(delay_get(num) == limit[num]) {
        return 1;
    }
    else{
        return 0;
    }
}

/**
    Name: interrupt vector 14

    Parameters: none

    Changes: increments count

    This interrupt vector increments both instances of count
    only if they are less than their respective limits.
*/
void __vector_14() __attribute__((signal, used, externally_visible));
void __vector_14(){
    if(count[0] < limit[0]){
        ++count[0];
    }
    if(count[1] < limit[1]){
        ++count[1];
    }
}
