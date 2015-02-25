/*
 * main.c
 *
 *  Created on: Feb 17, 2015
 *      Author: mdbalke
 */

#include <msp430.h>
#include "microcontroller.h"

#include "spi.h"

#define CS_LOW P1OUT &= ~CS
#define CS_HIGH P1OUT |= CS

volatile unsigned char TXData;
volatile unsigned char RXData;



int main(void) {

	mcu_setup();
	//spi_setup();


	WDTCTL = WDTPW + WDTHOLD;                           // Set slave reset
	P1DIR |= CS + WP + HOLD + BIT7;
	//P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select

	P1OUT |= HOLD;
	P1OUT |= CS;
	P1OUT |= WP;


	/*
	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

	TXData = 0x05;
	RXData = 0x00;

	CS_LOW;
	UCA0TXBUF = TXData;

	 __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
	 CS_HIGH;
	led_flash();

	//RXData = UCA0RXBUF;

	*/
	while(1);

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
	//volatile unsigned int i;

	while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

	//led_flash();
	RXData = UCA0RXBUF;

	//TXData++;

	//if(TXData >= 15){
		//led_flash();
		//led_flash();
		//led_flash();
	__bic_SR_register_on_exit(LPM0_bits);
	IE2 &= ~UCA0RXIE;
	//}

	//UCA0TXBUF = TXData;                     // Send next value

	//for (i = 30; i; i--);                     // Add time between transmissions to
}                                           // make sure slave can keep up

