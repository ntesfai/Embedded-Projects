/**     author: Nubian Tesfai

        Course: SER486, Arizona State University

        Instructor: Doug Sandy

        This C program utilizes the Atmega 328p
        EEPROM memory for the purpose of saving
        VPN as well as other factory related data.
*/
#include "eeprom.h"
#include "uart.h"
#include "led.h"

#define EEAR (*((unsigned volatile int*)0x41))
#define EEARH (*((unsigned volatile char*)0x42))  //High address
#define EEARL (*((unsigned volatile char*)0x41))  //Low address
#define EEDR (*((unsigned volatile char*)0x40))   //EEPROM Data Register
#define EECR (*((unsigned volatile char*)0x3F))   //EEPROM Control Register

#define EERE 0x1
#define EEPE 0x2
#define EERIE 0x8
#define EEMPE 0x4
#define WRITE 0xFD
#define DISABLE 0xF7
#define BUFSIZE 64
#define ADDRSHFT 0x8

//Method & variable declarations.
int eeprom_isbusy();

static unsigned char writebuf[BUFSIZE];
static unsigned char writesize;
static volatile unsigned char bufindex;
static volatile unsigned int writeaddr;
static volatile unsigned char write_busy = 0;


/**
    Name: eeprom_writebuf
    Parameters: unsigned int, unsigned char*, unsigned char
    Changes: Updates the eeprom write address, size and write busy
    flag. Copies data from buf to writebuf.
*/
void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size){
    writeaddr = addr;
    writesize = size;
    while(write_busy);  //A previous write is being performed.
    write_busy = 1;
    unsigned int index;
    for(index = 0;index < size; index++){
        writebuf[index] = buf[index];
    }
    EECR |= EERIE; //Write a 1 to the ready interrupt enable
}

/**
    Name: eeprom_readbuf
    Parameters: unsigned int, unsigned char*, unsigned char
    Changes: Reads data from the eeprom, address must be passed.
*/
void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size) {
    while(eeprom_isbusy());
    unsigned int index;
        for(index = 0; index < size; index++) {
            EEAR = addr;
            EECR |= EERE;
            buf[index] = EEDR; //halted for four instruction cycles
            addr++;
        }
}

/**
    Name: eeprom_isbusy
    Parameters: none
    Changes: Verifies if the write_busy flag is high.
*/
int eeprom_isbusy() {
    return write_busy == 1;
}

/**
    Name: vector_22
    Changes: Writes data to address specified to the eeprom. Amount
    of data is tracked with the bufindex and writesize variables.
*/
void __vector_22() __attribute__((signal, used, externally_visible));
void __vector_22() {
    if(bufindex < writesize) {
        EEAR = writeaddr;
        EEDR = writebuf[bufindex];
        EECR |= EEMPE;
        EECR |= EEPE;
        bufindex++;
        writeaddr++;
    }
    else if(bufindex >= writesize) {
        EECR = 0;
        write_busy = 0;
    }
}
