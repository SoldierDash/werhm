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

// Slave mode ifdef, else Master mode
//#define SLAVE_SPI

volatile char RXData;

void spi_setup() {

	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

	P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

}


unsigned spi_send(unsigned char out){

	UCA0TXBUF = out;

	//IE2 |= UCA0RXIE;
	__bis_SR_register(LPM0_bits + GIE);
	return RXData;
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	while (!(IFG2 & UCA0TXIFG));

	RXData = UCA0RXBUF;
	__bic_SR_register_on_exit(LPM0_bits);

}


