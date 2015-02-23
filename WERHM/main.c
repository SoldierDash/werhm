/*
 * main.c
 *
 *  Created on: Feb 17, 2015
 *      Author: mdbalke
 */

#include <msp430.h>
#include "microcontroller.h"
#include "interrupt.h"

#include "spi.h"

int main(void) {

	mcu_setup();
	spi_setup();

	WDTCTL = WDTPW + WDTHOLD;                           // Set slave reset
	P1DIR |= CS + WP + HOLD + BIT7;
	P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
	P1OUT |= CS;

	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

	unsigned char TXData = 0x01;
	unsigned char RXData = 0x00;

	UCA0TXBUF = TXData;

	return 0;
}

// Test for valid RX and TX character
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	volatile unsigned int i;

	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

	if (UCA0RXBUF == SLV_Data)                // Test for correct character RX'd
		P1OUT |= 0x01;                          // If correct, light LED
	else
		P1OUT &= ~0x01;                         // If incorrect, clear LED

	MST_Data++;                               // Increment master value
	SLV_Data++;                               // Increment expected slave value
	UCA0TXBUF = MST_Data;                     // Send next value

	for (i = 30; i; i--);                     // Add time between transmissions to
}                                           // make sure slave can keep up

