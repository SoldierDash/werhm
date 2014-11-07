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
	 * USIOE: USI Data output enable
	 */
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIOE; // Port, SPI master


#ifdef SLAVE_SPI
	/* Slave mode */
	USICTL0 &= ~USIMST;

#else

	/* USIMST: Enable master mode */
	USICTL0 |= USIMST;

	/*
	 * USI Clock Control Register
	 * USIDIV_x: Clock divider (2^x)
	 * USISSEL_x: Clock source (1:ACLK, 2/3:SMCLK)
	 */
	USICKCTL = USIDIV_4 + USISSEL_2;

	//TODO need to intialize USIIFLG=1?
#endif



	/*
	 * USISWRST: USI software reset (1->reset)
	 * When high, stops operation of counter and shift register
	 */
	USICTL0 &= ~USISWRST;

#ifdef SLAVE_SPI

	// Slave ready to read one byte
	USISRL = 0x00;
	USICNT = 8;

	USICTL1 |= USIIE;

	_bis_SR_register(GIE);
#endif
}

void
spi_tx(char data) {

	//printf("Test");
	// Disable interrupts
	//_bic_SR_register(GIE);

	_tx_sending = 1;

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

	/*
	 * USI Control Register 1
	 * USIIE: USI counter interrupt enable
	 * -When USIIFG=1 & USIIE=1, will trigger USI_VECTOR interrupt and SCLK will hold
	 * -When USIIFG=1 & USIIE=0, SCLK will hold
	 */
	USICTL1 |= USIIE;

	while(USICNT);

	// Enable interrupts and sleep
	//_bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=USI_VECTOR
__interrupt void
universal_serial_interface(void) {

#ifdef SLAVE_SPI
	// Pass recived to handler
	(*_spi_rx_handler)(USISRL);

	// Ready to recieve another byte
	USISRL = 0x00;
	USICNT = 8;
#else
	// Disable SPI interrupt
	USICTL1 &= ~USIIE;
#endif

	_bic_SR_register_on_exit(LPM0_bits);
}
