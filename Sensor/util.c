/*
 * util.c
 *
 *  Created on: Nov 17, 2014
 *      Author: mdbalke
 */

#include "util.h"
#include <msp430.h>

void blink_red() {
	volatile unsigned int j;
	for (j = 2; j != 0; j--) {
		P1OUT ^= BIT0;
		_delay_cycles(40000);
	}
}


void init_clk() {
	//WDT config
	WDTCTL = WDTPW + WDTHOLD;

	/*	TimerA config
	 * 	-Source from ACLK
	 * 	-Up mode
	 * 	-Enable TimerA interrupt
	 */
	TA0CCR0 = 32768;
	TA0CTL = TASSEL_1 + MC_1;
	TA0CCTL0 |= CCIE;

	/* Clock config
	 *		-/1 divider for ACLK
	 *		-Source from crystal
	 *		-12.5pF cap for crystal
	 */
	BCSCTL1 |= DIVA_0;
	BCSCTL2 |= SELM_2;
	BCSCTL3 |= XCAP_3;
}
