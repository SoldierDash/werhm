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
#include "microcontroller.h"

void (*_spi_rx_handler)(char);
char _tx_sending;

void
spi_setup(void (*spi_rx)(char)) {
	_spi_rx_handler = spi_rx;
	_tx_sending = 0;

	/* Set P1.7(SDI) P1.6(SDO) and P1.5(SCLK) to input */
	//P1DIR &= ~(BIT7 + BIT6 + BIT5);

	/*
	 * USI Control Register 0
	 * USIPEx: Enable SPI mode for pin 1.x
	 * -Pin must be set to input
	 * USIMST: Enable master mode
	 * USIOE: USI Data output enable
	 */
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIMST + USIOE; // Port, SPI master

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

	// Get argument for function to handle data reciept
}

void
spi_tx(char data) {
	// Disable interrupts
	_bic_SR_register(GIE);

	_tx_sending = 1;

	/*
	 * USI Shift Register Low
	 * -Lower 8 bits of shifted data
	 */
	USISRL = data;

	/*
	 * USI Count
	 * USICNT: Number of bits to rx/tx
	 */
	USICNT = 8;

	/*
	 * USI Control Register 1
	 * USIIE: USI counter interrupt enable
	 */
	USICTL1 |= USIIE;

	//TODO Try sleeping to test for interrupt waiting

	// Enable interrupts and sleep
	_bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=USI_VECTOR
__interrupt void
universal_serial_interface(void) {

	// Disable SPI interrupt
	USICTL1 &= ~USIIE;

	// If not waiting for tx reciept
	if(!_tx_sending) {
		// Pass recived to handler
		(*_spi_rx_handler)(USISRL);
	} else {
		_tx_sending = 0;
		// Switch off LPM0 mode
		_bic_SR_register_on_exit(LPM0_bits);
	}
}



