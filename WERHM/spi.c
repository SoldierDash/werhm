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

volatile char interrupt_rx; // Temporary register for storing rx from spi_interrupt

void spi_setup() {

	P1DIR |= CC1101_CS + FLASH_CS + BIT1;


	P3SEL |= BIT0 + BIT5 + BIT4;
	P3DIR |= BIT6;

	FLASH_DESELECT;
	CC1101_DESELECT;

	UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;
	UCA0BR1 = 0;
	UCA0MCTL = 0;
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

	P1OUT |= BIT1;
	P3OUT |= BIT6;
}

unsigned char spi_tx_am(unsigned char tx) {

	unsigned char out = 0;

	IFG2 &= ~UCA0RXIFG;
	UCA0TXBUF = tx;
	while (!(IFG2 & UCA0RXIFG));
	out = UCA0RXBUF;
	return out;
}

unsigned char spi_tx_lpm_iu(unsigned char tx) {

	unsigned char out = 0;

	IFG2 &= ~UCA0RXIFG;
	UCA0TXBUF = tx;
	while (!(IFG2 & UCA0RXIFG));
	out = UCA0RXBUF;
	return out;

}
