/**
    Author: Nubian Tesfai

    Course: SER486, Arizona State University

    Instructor: Doug Sandy

    This class uses the vpd struct to hold the data
    written to the EEPROM. Checksum is performed to
    ensure validity of the data.
*/
#include "util.h"
#include "eeprom.h"
#include "vpd.h"
#include "uart.h"
#define VPD_ADDR 0x000

int vpd_is_data_valid();
void vpd_write_defaults();

vpd_struct vpd;
static vpd_struct defaults = {"SER", "Nubian", "Tesfai", "12345", 0, {'N', 'u', 'b', 'T', 'e', 's'}, "USA", 0};

/**
    Name: vpd_init
    Parameters: none
    Changes: Reads from EEPROM to the vpd struct. Ensures
    data is valid with checksum
*/
void vpd_init() {
    while(eeprom_isbusy());
    eeprom_readbuf(VPD_ADDR, (unsigned char*)&vpd, sizeof(vpd));
    if(!vpd_is_data_valid()) {
        vpd_write_defaults();
        eeprom_readbuf(VPD_ADDR, (unsigned char*)&vpd, sizeof(vpd));
        uart_writestr("Reading from EEPROM\r\n");
    }
}

/**
    Name: vpd_is_data_valid
    Parameters: none
    Changes: Validates the data in vpd to ensure
    it is valid. Checks the token data member and
    that the size of the struct is correct.
*/
int vpd_is_data_valid() {
    const char* token = "SER";
    if((vpd.token[0] == token[0] && vpd.token[1] == token[1] && vpd.token[2] == token[2] ) &&
        is_checksum_valid((unsigned char*)&vpd, sizeof(vpd))) {
            return 1;
        }
    else {
        return 0;
    }
    /*if ((char*)vpd.token == token && util_is_checksum_valid((unsigned char*)&vpd, sizeof(vpd))) {
        return 1;
    }
    else
        return 0;*/
}

/**
    Name: vpd_write_defaults
    Parameters: none
    Changes: Writes the default values
    of vpd to the EEPROM. Updates the checksum beforehand.
*/
void vpd_write_defaults() {
    update_checksum((unsigned char*)&defaults, sizeof(defaults));
    eeprom_writebuf(VPD_ADDR, (unsigned char*)&defaults, sizeof(defaults));
}
