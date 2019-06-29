/*************************************************************************
* main.c
*
* This is template code for the SER486 final exam.  Students may use this
* as a starting point their final project.
*/

/**
    Name: Nubian Tesfai
    Course: SER486
    Instructor: Douglas Sandy
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
#include "parser.h"

#define HTTP_PORT       80	/* TCP port for HTTP */
#define SERVER_SOCKET   0


int main(void)
{
	/* Initialize the hardware devices
	 * uart
	 * led
	 * vpd
	 * config
     * log
     * rtc
     * spi
     * temp sensor
	 * W51 Ethernet controller
     * tempfsm
     */
     uart_init();
     led_init();
     vpd_init();
     config_init();
     log_init();
     rtc_init();
     spi_init();
     temp_init();
     W51_init();
     tempfsm_init();

     unsigned char retries = 3;
     char* mac_addr = "NubTes";
     char* model = "Nubian";
     char* manufac = "Tesfai";
     char* serial_no = "PHX623_ASU";
     int i = 0;
     for(;i < 6; i++){
        vpd.mac_address[i] = mac_addr[i];
        vpd.model[i] = model[i];
        vpd.manufacturer[i] = manufac[i];
        vpd.serial_number[i] = serial_no[i];
     }
    /* configure the W51xx ethernet controller prior to DHCP */
    unsigned char blank_addr[] = {0,0,0,0};
    W51_config(vpd.mac_address, blank_addr, blank_addr, blank_addr);

    /* loop until a dhcp address has been gotten */
    while (!dhcp_start(vpd.mac_address, 60000UL, 4000UL)) {}
    uart_writestr("local ip: ");uart_writeip(dhcp_getLocalIp());

    /* configure the MAC, TCP, subnet and gateway addresses for the Ethernet controller*/
    W51_config(vpd.mac_address, dhcp_getLocalIp(), dhcp_getGatewayIp(), dhcp_getSubnetMask());

	/* add a log record for EVENT_TIMESET prior to synchronizing with network time */
    log_add_record(EVENT_TIMESET);
    /* synchronize with network time */
    ntp_sync_network_time(retries); //Figure this out...
    /* add a log record for EVENT_NEWTIME now that time has been synchronized */
    log_add_record(EVENT_NEWTIME);
    /* start the watchdog timer */
    wdt_init();
    /* log the EVENT STARTUP and send and ALARM to the Master Controller */
    log_add_record(EVENT_STARTUP);
    alarm_send(EVENT_STARTUP);
    /* start the first temperature reading and wait 5 seconds before reading again
    * this long delay ensures that the temperature spike during startup does not
    * trigger any false alarms.
    */
    temp_start();
    delay_set(1,5000);

    while (1) {
        /* reset  the watchdog timer every loop */
        wdt_reset();
        /* if the temperature sensor delay is done, update the current temperature
        * from the temperature sensor, update the temperature sensor finite state
        * machine (which provides hysteresis) and send any temperature sensor
        * alarms (from FSM update).
        */
        if(delay_isdone(1)) {
            /* read the temperature sensor */
            int temp = temp_get();
            /* update the temperature fsm and send any alarms associated with it */
            tempfsm_update(temp, config.hi_alarm, config.hi_warn, config.lo_alarm, config.lo_warn);
            /* restart the temperature sensor delay to trigger in 1 second */
            temp_start();
            delay_set(1, 1000);
        }else if(socket_is_closed(SERVER_SOCKET))/*if the server socket is closed */ {
            /* if socket is closed, open it in passive (listen) mode */
            socket_open(SERVER_SOCKET, HTTP_PORT);
            socket_listen(SERVER_SOCKET);
        }else if (socket_recv_available(SERVER_SOCKET))/* if there is input to process */ {
            /* parse and process any pending commands */
            parser_response(parser_parse());
            socket_disconnect(SERVER_SOCKET);
        }else/* otherwise... */ {
          /* update any pending log write backs */
          log_update_noisr();
          /* update any pending config write backs */
          config_update_noisr();
       }
    }
	return 0;
}
