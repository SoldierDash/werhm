/*
 * SPI interface
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 */

#ifndef SPI_H_
#define SPI_H_

void spi_setup(void (*spi_rx)(char));

char spi_tx(char tx);

#endif /* SPI_H_ */
