/*
 * interrupt.c
 *
 *  Created on: Oct 25, 2014
 *      Author: Brandon
 */

#include <msp430.h>
#include "microcontroller.h"
#include "spi.h"

#pragma vector=PRAGMA_VECTOR
__interrupt void Port_1_ISR(void) {
	char input = P1IN;

	if(P1IFG & BIT3) {
		P1OUT ^= BIT0;
		spi_tx(0xFF);

		P1IFG &= ~BIT3;
	}

	//_\bic_SR_register_on_exit(LPM4_bits);
}
