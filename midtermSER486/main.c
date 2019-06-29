/*
 Author: Nubian Tesfai

 Course: SER486 Arizona State University

 Assignment: Midterm

 This file contains the methods implemented according to
 the requirements set on the midterm.
 */

#include "uart.h"

void delay(int num) {
    int i;
    for (i = 0; i < num; i++){}
}

int main(void)
{
    delay(50000);
    char cdata = 5;
    int idata = 5;
    long int lidata = 5;
    char *p1, *p2;
    p1 = (&lidata) + 1;
    p2 = (&lidata);
    uart_init();
    uart_writehex8(p1 - p2);
    uart_writestr("\r\n");
    p1 = (&idata) + 1;
    p2 = (&idata);
    uart_writehex8(p1 - p2);
    uart_writestr("\r\n");
    p1 = (&cdata) + 1;
    p2 = (&cdata);
    uart_writehex8(p1 - p2);
    uart_writestr("\r\n");

    //size_of(&data);
    uart_writestr("\r\n");
    //size_of(&idata);
    while(1)
    ;

    return 1;
}
