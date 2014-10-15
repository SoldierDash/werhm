/*
 * sensor_test.c
 *
 *  Created on: Oct 14, 2014
 *      Author: mdbalke
 */


#include <msp430.h> 
#include "SHT71.h"

void blink_red();
void blink_green();

volatile int read;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= (BIT0 + BIT6);
    P1OUT &= ~(BIT0 + BIT6);

    P1IE |= BIT3;
    P1IFG &= ~BIT3;

    read = 0;

    volatile unsigned int temperature;

	while(1){

		if(read){

			temperature = read_temperature();
			blink_green();
			read = 0;
		}

		_bis_SR_register(LPM4_bits + GIE);

	}
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	P1IFG &= ~BIT3;
	read = 1;
	_bic_SR_register_on_exit(LPM4_bits);

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
