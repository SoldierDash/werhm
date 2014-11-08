/*
 * SPI interface
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 */

#ifndef SPI_H_
#define SPI_H_

void spi_setup(void (*spi_rx)(char));

/* SPI Transmit in Low Power Mode, Interrupt Unsafe */
char spi_tx_lpm_iu(char tx);

/* SPI Transmit in Active Power mode (interrupt safe) */
char spi_tx_am(char tx);

#endif /* SPI_H_ */
