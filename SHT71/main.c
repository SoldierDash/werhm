
#include <msp430.h> 

#define SDA_PIN BIT4
#define SCL_PIN BIT5



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= (BIT0 + BIT6);
    P1OUT &= ~(BIT0 + BIT6);

    volatile float temperature;
    volatile float humidity;

	while(1){

		//humidity = read_humidity();

		temperature = read_temperature();

		//blink_red();

		blink_green();

		_delay_cycles(1000000);

	}
}
