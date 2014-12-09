
#include <msp430.h> 
#include "SHT71.h"


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= (BIT0 + BIT6 + BIT7);
    P1OUT &= ~(BIT0 + BIT6);

    P1OUT |= BIT7;


	while(1){

		read_humidity();
		read_temperature();

		blink_green();

		_delay_cycles(1000000);

	}
}
