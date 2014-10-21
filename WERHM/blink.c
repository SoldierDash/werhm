//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>

void setFrequency(int freq);
void doSleep();
void flashLED();

int main(void) {

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	setFrequency(0);

	P1DIR |= BIT0 + BIT6;					// Set P1.0 to output direction

	P1IE |= BIT3;
	P1IFG &= ~BIT3;

	while(1)
		doSleep();
}

void setFrequency(int freq) {

	/* 0.15Mhz */
	BCSCTL1 &= ~(BIT3 + BIT2 + BIT1 + BIT0);
	//BCSCTL1 |= BIT1;

	DCOCTL &= ~(BIT7 + BIT6 + BIT5);
	//DCOCTL |= BIT5 + BIT6;
}

void doSleep() {
	_bis_SR_register(LPM4_bits + GIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
	P1IFG &= ~BIT3;
	flashLED();

	//_\bic_SR_register_on_exit(LPM4_bits);
}

void flashLED() {
	P1OUT ^= 0x01;

	int j = 10;
	for(;j != 0; j--) {
		volatile unsigned int i;	// volatile to prevent optimization
		//volatile unsigned int j;

		P1OUT ^= 0x41;				// Toggle P1.0 using exclusive-OR


		i = 5000;					// SW Delay
		do i--;
		while(i != 0);
	}

	P1OUT = 0x00;
}
