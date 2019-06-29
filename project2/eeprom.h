#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

void eeprom_writebuf(unsigned int addr, unsigned char* buf, unsigned char size);

void eeprom_readbuf(unsigned int addr, unsigned char* buf, unsigned char size);

int eeprom_isbusy();

#endif
