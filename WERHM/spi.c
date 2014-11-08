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

// Slave mode ifdef, else Master mode
//#define SLAVE_SPI

void (*_spi_rx_handler)(char);

void
spi_setup(void (*spi_rx)(char)) {
	_spi_rx_handler = spi_rx;


#ifndef SLAVE_SPI
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

	//TODO Reset slave devices
#else

	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIOE; // Port, SPI slave

	USICKCTL |= USICKPL;



	USICTL0 &= ~USISWRST;

	// Slave ready to read one byte
	USISRL = 0x00;
	USICNT = 8;

	/*
	 * USI Control Register 1
	 * USIIE: USI counter interrupt enable
	 * -When USIIFG=1 & USIIE=1, will trigger USI_VECTOR interrupt and SCLK will hold
	 * -When USIIFG=1 & USIIE=0, SCLK will hold
	 */
	USICTL1 |= USIIE;

	// Start waiting for input
	_bis_SR_register(LPM0_bits + GIE);
#endif
}

void
spi_tx_lpm_iu(char data) {
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
}

void
spi_tx_am(char data) {
	USISRL = data;

	USICNT = 8;

	// Wait until IFG is set
	while(!(USICTL1 & USIIFG));

	led_flash();
}

#pragma vector=USI_VECTOR
__interrupt void
universal_serial_interface(void) {

#ifndef SLAVE_SPI
	// Disable USI interrupt
	USICTL1 &= ~USIIE;

	led_flash();

	//(*_spi_rx_handler)(USISRL);

	// Wake from LPM0
	_bic_SR_register_on_exit(LPM0_bits);

#else
	// Flash LED to indicate reciept
	led_flash();

	// Ready to recieve another byte
	USISRL = 0x00;
	USICNT = 8;
#endif
}


