/**
    Author: Nubian Tesfai
    This program calculates the difference between each char
    in each pre-defined string. Each of the strings are valid URIs.
 */
#include "config.h"
#include "delay.h"
#include "dhcp.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "spi.h"
#include "uart.h"
#include "vpd.h"
#include "temp.h"
#include "socket.h"
#include "alarm.h"
#include "wdt.h"
#include "tempfsm.h"
#include "eeprom.h"
#include "ntp.h"
#include "w51.h"

/**
    Name: Parse
    Param: char*
    This method takes a char* parameter which iterates through each
    char subtracting its value from the result variable.
*/

unsigned int test_results[20];

unsigned int parse(char* uri) {
    static unsigned char index = 0;
    unsigned int result = 65535;
    unsigned int i = 0;
    char letter = uri[i];
    while(letter != 0){
        result -= letter;
        letter = uri[++i];
    }
    test_results[index++] = result;
    return result;
}

/**
    Name: duplicate_val
    Param: int[]
    This method checks every int in the array
    for any duplicate values. There should be none.
*/
int duplicate_val(unsigned int *test_results){
    unsigned int i = 0;
    unsigned int j = 1;
    for(i = 0;i < 20; i++){
        for(j = 1;j < 20; j++){
            if(i != j && test_results[i] == test_results[j]){
                uart_writestr("Duplicate values found at "); uart_writedec32(i); uart_writestr(", "); uart_writedec32(j);
                return -1;
            }

        }
    }
    return 0;
}
int main(void)
{
    uart_init();
    char* get = "GET";
    char* put = "PUT";
    char* del = "DELETE";
    char* device = "GET /device HTTP/1.1";
    //char* wrong_device = "PUT /deevic HTTP/1.1"; //This solution won't work for characters that are out of order
    char* device_VPD = "GET /device/vpd HTTP/1.1";
    char* device_VPD_model ="GET /device/vpd/model HTTP/1.1";
    char* device_VPD_man ="GET /device/vpd/manufacturer HTTP/1.1";
    char* device_VPD_ser = "GET /device/vpd/serial_number HTTP/1.1";
    char* device_VPD_date = "GET /device/vpd/manufacture_date HTTP/1.1";
    char* device_VPD_mac = "GET /device/vpd/mac_address HTTP/1.1";
    char* device_VPD_country = "GET /device/vpd/country_code HTTP/1.1";
    char* device_config = "GET /device/config HTTP/1.1";
    char* device_config_crithi = "GET /device/config/tcrit_hi HTTP/1.1";
    char* device_config_critlo = "GET /device/config/tcrit_lo HTTP/1.1";
    char* device_config_warnhi = "GET /device/config/twarn_hi HTTP/1.1";
    char* device_config_warnlo = "GET /device/config/twarn_lo HTTP/1.1";

    char* device_temp = "GET /device/temp HTTP/1.1";
    char* device_temp_val = "GET /device/temp/value HTTP/1.1";
    char* device_temp_st = "GET /device/temp/state HTTP/1.1";
    char* device_state = "GET /device/state HTTP/1.1";
    char* device_log = "DELETE /device/log HTTP/1.1";


    uart_writestr("Results: \n\n\r");
    uart_writestr("GET: "); uart_writedec32(parse(get));
    uart_writestr("\n\r");
    uart_writestr("PUT: "); uart_writedec32(parse(put));
    uart_writestr("\n\r");
    uart_writestr("DELETE: "); uart_writedec32(parse(del));
    uart_writestr("\n\r");
    uart_writestr("/device: "); uart_writedec32(parse(device));
    uart_writestr("\n\r");
    //uart_writestr("/deevic"); uart_writedec32(parse(wrong_device));//This produces the same result
    //uart_writestr("\n\r");
    uart_writestr("/device/vpd: "); uart_writedec32(parse(device_VPD));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/model "); uart_writedec32(parse(device_VPD_model));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/manufacturer: "); uart_writedec32(parse(device_VPD_man));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/serial_number: "); uart_writedec32(parse(device_VPD_ser));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/manufacture_date: "); uart_writedec32(parse(device_VPD_date));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/mac_address: "); uart_writedec32(parse(device_VPD_mac));
    uart_writestr("\n\r");
    uart_writestr("/device/vpd/country_code: "); uart_writedec32(parse(device_VPD_country));
    uart_writestr("\n\r");
    uart_writestr("/device/config: "); uart_writedec32(parse(device_config));
    uart_writestr("\n\r");
    uart_writestr("/device/config/tcrit_hi: "); uart_writedec32(parse(device_config_crithi));
    uart_writestr("\n\r");
    uart_writestr("/device/config/tcrit_lo: "); uart_writedec32(parse(device_config_critlo));
    uart_writestr("\n\r");
    uart_writestr("/device/config/twarn_hi: "); uart_writedec32(parse(device_config_warnhi));
    uart_writestr("\n\r");
    uart_writestr("/device/config/twarn_lo: "); uart_writedec32(parse(device_config_warnlo));
    uart_writestr("\n\r");
    uart_writestr("/device/temp: "); uart_writedec32(parse(device_temp));
    uart_writestr("\n\r");
    uart_writestr("/device/temp/value: "); uart_writedec32(parse(device_temp_val));
    uart_writestr("\n\r");
    uart_writestr("/device/temp/state: "); uart_writedec32(parse(device_temp_st));
    uart_writestr("\n\r");
    uart_writestr("/device/state: "); uart_writedec32(parse(device_state));
    uart_writestr("\n\r");
    uart_writestr("/device/log: "); uart_writedec32(parse(device_log));
    duplicate_val(test_results);

    while(1)
    ;

    return 0;
}
