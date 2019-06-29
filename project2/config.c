/**
    Author: Nubian Tesfai

    Course: SER486, Arizona State University

    Instructor: Doug Sandy

    This class holds the config data
    of the AVR. Checksum ensures the data
    in the config struct is valid.
*/
#include "eeprom.h"
#include "util.h"
#include "config.h"

#define CONFIG_ADDR 0x040

int config_is_data_valid();
void config_write_defaults();

config_struct config;
static config_struct defaults = {"ASU", 0x3FF, 0x3FE, 0x0000, 0x0001, 0, {192,168,1,100}, 0};
static unsigned char modified;

/**
    Name: config_init
    Parameters: none
    Changes: Initializes the config variable with values that
    are valid for its data members.
*/
void config_init(){
    while(eeprom_isbusy());
    eeprom_readbuf(CONFIG_ADDR, (unsigned char*)&config, sizeof(config));
    if(!config_is_data_valid()){
        config_write_defaults();
        eeprom_readbuf(CONFIG_ADDR, (unsigned char*)&config, sizeof(config));
    }
}

/**
    Name: config_update
    Parameters: none
    Changes: Writes to the EEPROM if the config data
    is updated.
*/
void config_update(){
    if(eeprom_isbusy() || !modified)
        return;
    else {
        eeprom_writebuf(CONFIG_ADDR, (unsigned char*)&config, sizeof(config));
        modified = 0;
    }
}

/**
    Name: config_is_data_valid
    Parameters: None
    Changes: Ensures the size of the struct is correct and the token
    value is valid.
*/
int config_is_data_valid() {
    const char* token = "ASU";
    if((config.token[0] == token[0] && config.token[1] == token[1] && config.token[2] == token[2])
 && is_checksum_valid((unsigned char*)&config, sizeof(config)))
        return 1;
    else
        return 0;
}

/**
    Name: config_set_modified
    Parameters: none
    Changes: If the config data has been modified
    update modified value;
*/
void config_set_modified() {
    modified = 1;
}


/**
    Name: config_write_defaults
    Parameters: none
    Changes: Updates the checksum value for the default struct.
    Writes the values to the EEPROM.
*/
void config_write_defaults() {
    update_checksum((unsigned char*)&defaults, sizeof(defaults));
    eeprom_writebuf(CONFIG_ADDR, (unsigned char*)&defaults, sizeof(defaults));
}
