/*
 * microcontroller.c
 * Driver for the MSP-EXP430G2
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */


#include <msp430.h>
#include "microcontroller.h"

#define INTERRUPT_PRAGMA

void
mcu_setup() {
	/* Stop watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

	/* 1MHZ clock */
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1DIR |= LED_PIN;
	P1OUT = 0;

	P2DIR |= BIT3;
	P2OUT |= BIT3;
	/* Enable interrupt */
	_bis_SR_register(GIE);
}


void
led_flash() {

	int j = 6;
	for (;j != 0; j--) {
		volatile unsigned int i;
		P1OUT ^= LED_PIN;
		_delay_cycles(30000);
	}
	P1OUT &= ~LED_PIN;
}
