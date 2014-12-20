/*
 * microcontroller.c
 * Driver for the MSP-EXP430G2
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */


#include <msp430.h>
#include "microcontroller.h"
#include "cc1101.h"

#define INTERRUPT_PRAGMA

void mcu_setup() {
	/* Stop watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

	/**
	 * Set frequency
	 * 0.15Mhz
	 */
	//BCSCTL1 &= ~(BIT3 + BIT2 + BIT1 + BIT0); //BCSCTL1 |= BIT1;
	//DCOCTL &= ~(BIT7 + BIT6 + BIT5); //DCOCTL |= BIT5 + BIT6;

	/* Set pins for output */
	P1DIR |= BIT0; // Enable P1.0

	P1OUT = 0x00;

	/* Set pins for input */
	//P1DIR &= ~BIT3; // Enable none

	/* Set interrupt pins */
	//P1IE |= BIT3; // Set P1.3 to interrupt

	/* Clear interrupt pins */
	//P1IFG &= ~BIT3;

	rx_flag = 0;

	/* Enable interrupt */
	_bis_SR_register(GIE);
}

void mcu_sleep_gie() {
	/**
	 * Set status register to sleep with interrupt
	 *  LMP4_bits = Low power mode 4 (disable everything)
	 *  GIE = Global interrupts enabled
	 */
	_bis_SR_register(LPM4_bits + GIE);
}

void mcu_wait_gie() {
	_bis_SR_register(LPM3_bits + GIE);
}

void led_flash() {

	int j = 1;
	for (;j != 0; j--) {
		volatile unsigned int i;	// volatile to prevent optimization
		//volatile unsigned int j;

		P1OUT ^= BIT0;				// Toggle P1.0 using exclusive-OR


		i = 5000;					// SW Delay
		do {
			i--;
		} while(i != 0);
	}


	P1OUT &= ~BIT0;
}

void blink_red(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT0;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT0;
}

void blink_green(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT6;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT6;
}



