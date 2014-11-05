/*
 * SPI library for MSP430f2012/3 over USI
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 */

#include <msp430.h>

void
spi_setup() {
	// Disable USI reset USISWRST
	// Reset for USIIFG, USISTTIFG, USISTP, USIAL

	// SPI mode with USII2C = 0
	// SPI master with USIMST master bit
	// -> SCLK must be

	// Activate USI port with corresponding USIPEx bits
	USICTL0;


	// Get argument for function to handle data reciept
}

void
spi_tx() {

}

void
spi_rx() {
	char data = USISR;
}



