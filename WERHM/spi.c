/*
 * SPI library for MSP430f2012/3 over USI
 *
 * USI output = P1.6
 * USI input = P1.7
 * USI SCLK = P1.5
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 *
 *
 */

#include <msp430.h>

void (*_spi_rx_handler)(char);

void
spi_setup(void (*spi_rx)(char)) {
	_spi_rx_handler = spi_rx;

	/* Set P1.7(SDI) P1.6(SDO) and P1.5(SCLK) to input */
	P1DIR &= ~(BIT7 + BIT6 + BIT5);

	/*
	 * USI Control Register 0
	 * USIPEx: Enable SPI mode for pin 1.x
	 * -Pin must be set to input
	 * USIMST: Enable master mode
	 * USIOE: USI Data output enable
	 */
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIMST + USIOE; // Port, SPI master

	/*
	 * USI Control Register 1
	 * USIIE: USI counter interrupt enable
	 */
	USICTL1 |= USIIE;

	/*
	 * USI Clock Control Register
	 * USIDIV_x: Clock divider (2^x)
	 * USISSEL_x: Clock source (1:ACLK, 2/3:SMCLK)
	 */
	USICKCTL = USIDIV_4 + USISSEL_2;

	/*
	 * USISWRST: USI software reset (1->reset)
	 */
	USICTL0 &= ~USISWRST;



	// Reset for USIIFG, USISTTIFG, USISTP, USIAL

	// SPI mode with USII2C = 0
	// SPI master with USIMST master bit
	// -> SCLK must be

	// Activate USI port with corresponding USIPEx bits
	USICTL0;


	// Get argument for function to handle data reciept
}

void
spi_tx(char tx) {
	/*
	 * USI Shift Register Low
	 * -Lower 8 bits of shifted data
	 */
	USISRL = tx;

	/*
	 * USI Count
	 * USICNT: Number of bits to rx/tx
	 */
	USICNT = 8;

	//Start waiting for an interrupt from USI_VECTOR
}

char
spi_rx() {
	char data = USISR;
}

#pragma vector=USI_VECTOR
__interrupt void
universal_serial_interface(void) {

}



