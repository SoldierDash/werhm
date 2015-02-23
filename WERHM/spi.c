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

volatile char interrupt_rx; // Temporary register for storing rx from spi_interrupt

void spi_setup() {

	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	P1OUT = 0x00;                             // P1 setup for LED
	P1DIR |= 0x01;                            //
	P3OUT = 0x40;                             // Set slave reset
	P3DIR |= CS + WP + HOLD + BIT7;
	P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

}


