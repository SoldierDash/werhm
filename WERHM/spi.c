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
#include "spi.h"

volatile unsigned char interrupt_rx; // Temporary register for storing rx from spi_interrupt

void
spi_setup() {

	P1SEL |= P1USI_SCLK + P1USI_SO + P1USI_SI;

	/*
	 * USI Control Register 0
	 * USIPEx: Enable SPI mode for pin 1.x
	 * -Pin must be set to input
	 * USIOE: USI Data output enable
	 */
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIOE + USIMST; // Port, SPI master

	/*
	 * USI Clock Control Register
	 * USIDIV_x: Clock divider (2^x)
	 * USISSEL_x: Clock source (1:ACLK, 2/3:SMCLK)
	 * -Automatically SCLK if slave
	 */
	USICKCTL = USIDIV_4 + USISSEL_2 + USICKPL;

	/*
	 * USISWRST: USI software reset (1->reset)
	 * When high, stops operation of counter and shift register
	 */
	USICTL0 &= ~USISWRST;

	//Set SDO low and remove extra bit errata
	USISRL = 0x00;
	USICNT = 1;

}

unsigned char spi_tx_lpm(unsigned char data) {
	_bic_SR_register(GIE);

	/*
	 * USI Shift Register Low
	 * -Lower 8 bits of shifted data
	 */
	USISRL = data;

	/*
	 * USI Count
	 * USICNT: Number of bits to rx/tx
	 * -Automatically clears USIIFG
	 * -Sets USIIFG when USICNT=0
	 */
	USICNT = 8;

	// Wait until IFG is set
	USICTL1 |= USIIE;

	_bis_SR_register(LPM0_bits + GIE);

	return interrupt_rx;
}

unsigned char spi_tx(unsigned char data) {
	USISRL = data;

	USICNT = 8;

	// Wait until IFG is set
	while(!(USICTL1 & USIIFG));

	return USISRL;
}

#pragma vector=USI_VECTOR
__interrupt void
universal_serial_interface(void) {
	// Disable USI interrupt
	USICTL1 &= ~USIIE;

	// Store message
	interrupt_rx = USISRL;

	// Wake from LPM0
	_bic_SR_register_on_exit(LPM0_bits);
}


