
#include <msp430.h> 
#include "SHT71.h"

void blink_red();
void blink_green();

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= 0xFF;
    P1OUT &= ~(BIT0 + BIT6);

    volatile unsigned int temperature;

	while(1){

		temperature = read_temperature();

		blink_red();

		_delay_cycles(1000000);

	}
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
