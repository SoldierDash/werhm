/*
 * SPI interface
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 */

#ifndef SPI_H_
#define SPI_H_

// Port1 USI pins
#define P1USI_SCLK	BIT5
#define P1USI_SI	BIT6	//Slave in (master out)
#define P1USI_SO	BIT7	//Slave out (master in)

void spi_setup();

/* !INTERRUPT UNSAFE! - do not call inside interrupt
 * SPI Transmit in Low Power Mode */
unsigned char spi_tx_lpm(unsigned char data);

/* SPI Transmit in Active Power mode (interrupt safe) */
unsigned char spi_tx(unsigned char data);

#endif /* SPI_H_ */
