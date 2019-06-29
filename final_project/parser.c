/**
    Author: Nubian Tesfai
    Instructor: Douglas Sandy
    Course: SER486, Arizona State University

    This class defines the functionality for parsing
    an HTTP request from the master controller to the AVR.
    Data is received from a socket and must be parsed in order
    to respond accordingly.
*/

//Import needed classes
#include "socket.h"
#include "config.h"
#include "vpd.h"
#include "uart.h"
#include "parser.h"
#include "rtc.h"
#include "temp.h"
#include "tempfsm.h"
#include "log.h"
#include "util.h"



#define PARSE_SOCKET 0

static unsigned char letter;
static char* state;
static int new_temp;

/**
    Name: parser_parse
    Param: n/a
    This method reads bytes from the input stream and
    subtracts from the int variable "result". This should generate a
    unique integer value for a well formed HTTP request.
*/
int parser_parse() {
    unsigned int result = 65535;
    //Read the request line
    while(socket_recv_compare(PARSE_SOCKET,"\r\n") == 0) {
        socket_recv(PARSE_SOCKET, &letter, 1);
        uart_writechar(letter);
        result -= letter;
    }

    while(socket_received_line(PARSE_SOCKET)){
    while(socket_recv_available(PARSE_SOCKET)){  //While there are bytes available in a line
        if(socket_recv_compare(PARSE_SOCKET,"<body>\r\n")){
            if(socket_recv_compare(PARSE_SOCKET,"-")){
                socket_recv_int(PARSE_SOCKET, &new_temp);
                new_temp = new_temp - (new_temp * 2);
                break;
            }
            else{
                socket_recv_int(PARSE_SOCKET, &new_temp);
                break;
                }
        }
        socket_recv(PARSE_SOCKET, &letter, 1);
        uart_writechar(letter);
    }
    }
    /**while(socket_recv_available(PARSE_SOCKET)){
        socket_flush_line(PARSE_SOCKET);
    }
    */

    //If there are additional bytes read as well
    //If there is no number to be read on a PUT, then the request is invalid
    uart_writestr("Result: ");
    uart_writedec32(result);
    return result;
}

/**
    Name: parser_print_nbsp
    Param: n/a
    This method writes &nbsp to the output stream.
    This helps to save memory when writing the HTTP response.
*/
void parser_print_nbsp(unsigned char times){
    unsigned char i = 0;
    for(;i < times; i++){
        socket_writestr(PARSE_SOCKET, "&nbsp");
    }
}

/**
    Name: parser_print_OK
    Param: n/a
    Writes the OK response header to the socket.
*/

void parser_print_OK() {
    socket_writestr(PARSE_SOCKET, "HTTP/1.1 200 OK\r\nConnection-Type: text/html\r\nConnection: Closed\r\n\r\n");
}

/**
    Name: parser_html_header
    Param:n/a
    Writes the html header to the socket.
*/
void parser_html_header() {
    socket_writestr(PARSE_SOCKET, "<!DOCTYPE HTML>\r\n<html><body>/device<br />");
}

/**
    Name: parser_vpd
    Param: n/a
    Writes the entire vpd to the socket.
*/
void parser_vpd() {
    socket_writestr(PARSE_SOCKET, "/vpd<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/model");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.model);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/manufacturer");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.manufacturer);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/serial_number");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.serial_number);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/manufacture_date");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, rtc_num2datestr(vpd.manufacture_date));
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/mac_address");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, (char*)vpd.mac_address);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);


    socket_writestr(PARSE_SOCKET, "/country_code");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.country_of_origin);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_config
    Param: n/a
    Writes the config to the socket.
*/
void parser_config() {
    socket_writestr(PARSE_SOCKET, "/config <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/tcrit_hi");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.hi_alarm);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/twarn_hi");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.hi_warn);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/tcrit_lo");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.lo_alarm);
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/twarn_lo");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.lo_warn);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_temp
    Param: n/a
    Writes the entire temp to the socket.
*/
void parser_temp(){
    int temp = temp_get();
    socket_writestr(PARSE_SOCKET, "/temp <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/value");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, temp_get());
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);

    if(temp >= config.hi_alarm)
        state = "CRIT_HI";
    else if(temp < config.hi_alarm && temp >= config.hi_warn)
        state= "WARN_HI";
    else if(temp <= config.lo_alarm)
        state = "CRIT_LO";
    else if(temp > config.lo_alarm && temp <= config.lo_warn)
        state = "WARN_HI";
    else
        state = "NORMAL";

    socket_writestr(PARSE_SOCKET, "/state");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, state);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_log
    Param: n/a
    Writes the entire log to the socket.
*/
void parser_log(){
    unsigned char log_num = 0;
    unsigned char entries = log_get_num_entries();
    unsigned long* time = 0;
    unsigned char* event_num = 0;
    socket_writestr(PARSE_SOCKET, "/log");
    socket_writestr(PARSE_SOCKET, "<br />");
    for(;log_num < entries; log_num++) {
        parser_print_nbsp(10);
        socket_writechar(PARSE_SOCKET, '/'); socket_writedec32(PARSE_SOCKET, log_num);
        parser_print_nbsp(15);
        socket_writestr(PARSE_SOCKET, "/timestamp");
        rtc_num2datestr(log_get_record(log_num, time, event_num));
        parser_print_nbsp(1); socket_writequotedstring(PARSE_SOCKET, rtc_num2datestr(*time));
        socket_writestr(PARSE_SOCKET, "<br />");
        parser_print_nbsp(15);
        socket_writestr(PARSE_SOCKET, "/eventnum");
        parser_print_nbsp(1); socket_writedec32(PARSE_SOCKET, (unsigned int)*event_num);
        socket_writestr(PARSE_SOCKET, "<br />");
    }
}

/**
    Name parser_state
    Param: n/a
    Write the device state to the socket.
*/
void parser_state() {
    socket_writestr(PARSE_SOCKET, "/state");
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);
    socket_writestr(PARSE_SOCKET, "healthy");
}

void parser_state_PUT(){
    socket_writestr(PARSE_SOCKET, "/state");
    socket_writestr(PARSE_SOCKET, "<br />");
    parser_print_nbsp(10);
    socket_writestr(PARSE_SOCKET, "reset");
}

/**
    Name: parser_vpd_manufacturer
    Param: n/a
    Write the device manufacturer to the socket.
*/
void parser_vpd_manufacturer(){
    socket_writestr(PARSE_SOCKET, "/vpd<br/>");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/manufacturer");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.manufacturer);
    socket_writestr(PARSE_SOCKET, "<br/>");
}

void parser_vpd_model(){
    socket_writestr(PARSE_SOCKET, "/vpd<br/>");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/model");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.model);
    socket_writestr(PARSE_SOCKET, "<br/>");
}

/**
    Name parser_vpd_serial
    Param: n/a
    Write the device serial number to the socket.
*/
void parser_vpd_serial(){
    socket_writestr(PARSE_SOCKET, "/vpd<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/serial_number");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.serial_number);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_vpd_manufacture_date
    Param: n/a
    Write the manufacture date to the socket.
*/

void parser_vpd_manufacture_date(){
    socket_writestr(PARSE_SOCKET, "/vpd<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/manufacture_date");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, rtc_num2datestr(vpd.manufacture_date));
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_vpd_mac
    Param: n/a
    Write the mac address to the socket.
*/
void parser_vpd_mac(){
    socket_writestr(PARSE_SOCKET, "/vpd<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/mac_address");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, (char*)vpd.mac_address);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_vpd_cc
    Param: n/a
    Write the country code to the socket.
*/
void parser_vpd_cc(){
    socket_writestr(PARSE_SOCKET, "/vpd<br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/country_code");
    parser_print_nbsp(1);
    socket_writequotedstring(PARSE_SOCKET, vpd.country_of_origin);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_config_tcrit_hi
    Param: n/a
    Write the crit high temp to the socket.
*/
void parser_config_tcrit_hi(){
    socket_writestr(PARSE_SOCKET, "/config <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/tcrit_hi");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.hi_alarm);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parse_config_twarn_lo
    Param: n/a
    Write the low warning to the socket
*/
void parser_config_twarn_lo(){
    socket_writestr(PARSE_SOCKET, "/config <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/twarn_lo");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.lo_warn);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_config_twarn_hi
    Param: n/a
    Writes the high warning to the socket
*/
void parser_config_twarn_hi(){
    socket_writestr(PARSE_SOCKET, "/config <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/twarn_hi");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.hi_warn);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_config_tcrit_lo
    Param: n/a
    Write the low critical temp to the socket
*/
void parser_config_tcrit_lo(){
    socket_writestr(PARSE_SOCKET, "/config <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/tcrit_lo");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, config.lo_alarm);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_temp_val
    Param: n/a
    Write the temperature value to the socket
*/
void parser_temp_val(){
    socket_writestr(PARSE_SOCKET, "/temp <br />");
    parser_print_nbsp(10);

    socket_writestr(PARSE_SOCKET, "/value");
    parser_print_nbsp(1);
    socket_writedec32(PARSE_SOCKET, temp_get());
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_temp_state
    Param: n/a
    Write the temperature state to the socket
*/
void parser_temp_state(){
    int temp = temp_get();
    socket_writestr(PARSE_SOCKET, "/temp <br />");
    parser_print_nbsp(10);

    if(temp >= config.hi_alarm)
        state = "CRIT_HI";
    else if(temp < config.hi_alarm && temp >= config.hi_warn)
        state= "WARN_HI";
    else if(temp <= config.lo_alarm)
        state = "CRIT_LO";
    else if(temp > config.lo_alarm && temp <= config.lo_warn)
        state = "WARN_HI";
    else
        state = "NORMAL";

    socket_writestr(PARSE_SOCKET, "/value");
    parser_print_nbsp(1);
    //socket_writedec32(PARSE_SOCKET, temp_get());
    socket_writequotedstring(PARSE_SOCKET, state);
    socket_writestr(PARSE_SOCKET, "<br />");
}

/**
    Name: parser_response
    Param: n/a
    This method determines the correct response based
    on the previous contents of the input stream
*/
void parser_response(unsigned int id) {
    switch(id){
        case 64065: ///GET /device
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd();
            parser_print_nbsp(5); parser_config();
            parser_print_nbsp(5); parser_temp();
            parser_print_nbsp(5); parser_log();
            parser_print_nbsp(5); parser_state();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 63688: ///GET /device/vpd
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        ///Write another case for the vpd model
        case 63112:
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_model();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62348: ///GET /device/vpd/manufacturer
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_manufacturer();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62257: ///GET /device/vpd/serial_number
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_serial();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 61953: ///GET /device/vpd/manufacture_date
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_manufacture_date();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62499: ///GET /device/vpd/mac_address
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_mac();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62347: ///GET /device/vpd/country_code
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_vpd_cc();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 63388: ///GET /device/config
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_config();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62487: ///GET /device/config/tcrit_hi
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_config_tcrit_hi();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62481: ///GET /device/config/twarn_hi
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_config_twarn_hi();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62471:  ///GET /device/config/twarn_lo
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_config_twarn_lo();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62477: ///GET /device/config/tcrit_lo
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_config_tcrit_lo();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 63580: ///GET /device/temp
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_temp();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62992: ///GET /device/temp/value
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_temp_val();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 62988: ///GET /device/temp/state
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_temp_state();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 63473: ///GET /device/state
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_state();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
        case 63696: ///GET/ device/log
            parser_print_OK();
            parser_html_header();
            parser_print_nbsp(5); parser_log();
            socket_writestr(PARSE_SOCKET, "</body>\r\n</html>\r\n\r\n");
            break;
            //Code for the PUT requests
        case 62462: ///PUT /device/config/tcrit_hi
            parser_print_OK();
            config.hi_alarm = new_temp;
            //config_set_modified();
            break;
        case 62452: ///PUT /device/config/tcrit_lo
            parser_print_OK();
            config.lo_alarm = new_temp;
            //config_set_modified();
            break;
        case 62456: ///PUT /device/config/twarn_hi
            parser_print_OK();
            config.hi_warn = new_temp;
            //config_set_modified();
            break;
        case 62446: ///PUT /device/config/twarn_lo
            parser_print_OK();
            config.lo_warn = new_temp;
            //config_set_modified();
            break;
        case 63448: ///PUT /device/state
            parser_print_OK();
            parser_state_PUT();
            break;
        default:
            socket_writestr(PARSE_SOCKET, "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n");
    }
    if(id == 62462 || id == 62452 || id == 62456 || id == 62446) {
        config_set_modified();
        //config_update_noisr();
        //update_checksum((unsigned char*)&config, sizeof(config));
    }
}
