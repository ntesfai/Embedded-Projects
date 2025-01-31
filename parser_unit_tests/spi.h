/********************************************************
 * socket.h
 *
 * this file provides function declarations for SER486
 * spi (serial peripheral interconnect) class member functions.
 *
 * Author:   Doug Sandy
 * Date:     4/18/2018
 * Revision: 1.0
 *
 * Copyright(C) 2018, Arizona State University
 * All rights reserved
 *
 */
#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

/* bit definitions for SPI Control Register 0 */
#define SPR00   0
#define SPR01   1
#define CPHA0   2
#define CPOL0   3
#define MSTR0   4
#define DORD0   5
#define SPE0    6
#define SPIE0   7

/* bit definitions for SPI Status Register 0 */
#define SPI2X0  1
#define WCOL0   6
#define SPIF0   7


/* Initialize the SPI library */
void spi_init();

/* allocate the spi bus to a specific piece of hardware - dont allow other
* hardware to access the SPI bus until the transaction is complete.
*/
inline void spi_begin_transfer(unsigned char spcr,unsigned char spsr);

/* Write to the SPI bus (MOSI pin) and also receive (MISO pin) */
inline unsigned char spi_transfer(unsigned char data);

/* after performing a series of transactions, release the spi bus so
* that other devices can use it
*/
inline void spi_end_transfer(void);

#endif // SPI_H_INCLUDED
