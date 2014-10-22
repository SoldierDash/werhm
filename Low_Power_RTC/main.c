#include <msp430.h>

//Alarm values for timerA ISR
#define ALARM_SECONDS 10
#define ALARM_MINUTES 0
#define ALARM_HOURS 0

volatile unsigned int seconds, minutes, hours, alarm;

/*
 *	Program to implement a real-time clock by putting the device in low power mode and waking up once every second
 *
 */

void main() {

	//WDT config
	 WDTCTL = WDTPW + WDTHOLD;

	 //Port1 config
	 P1DIR = 0xFF;
	 P1OUT = 0;

	 /*
	  * TimerA config
	  * 	-Source from SMCLK
	  * 	-Up mode
	  * 	-Compare at 32768 (1 second on crystal)
	  * 	-Enable TimerA interrupt
	  */
	 TA0CCR0 = 32768;
	 TA0CTL = TASSEL_2 + MC_1;
	 TA0CCTL0 |= CCIE;


	 /*
	  * Clock config
	  *		-/1 divider for ACLK
	  *		-/2 divider for MCLK + XT2CLK source
	  *		-12.5pF cap for crystal
	  *		-Source from crystal
	  */
	 BCSCTL1 |= DIVA_0;
	 BCSCTL2 |= DIVM_1 + SELM_2;
	 BCSCTL2 &= ~SELS;
	 BCSCTL3 |= XCAP_3 + LFXT1S_0;

	 seconds = 0;
	 minutes = 0;
	 hours = 0;

	 alarm = 0;

	 while(1){

		 //Put CPU to sleep in LPM mode 3, enable global interrupt
		 _bis_SR_register(LPM3_bits + GIE);

		 if(alarm){
			 seconds = 0;
			 minutes = 0;
			 hours = 0;
			 alarm = 0;

			 P1OUT |= BIT0;
		 }else{
			 P1OUT &= ~ BIT0;
		 }

	 }
}


/*
 * ISR for timerA interrupt vector
 * 		-TimerA is set to UP mode, so interrupt occurs when value of timer reaches value of register TA0CCR0
 *
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMERA0_ISR(){

	//Blink green LED every second
	unsigned int j;
	for(j = 2; j !=0; j--){
		P1OUT ^= BIT6;
		_delay_cycles(40000);
	}

	//Increment time
	seconds++;
	if(seconds >= 60){
		minutes++;
		seconds = 0;
		if(minutes >= 60){
			hours++;
			minutes = 0;
		}
	}


	if(seconds == ALARM_SECONDS && minutes == ALARM_MINUTES && hours == ALARM_HOURS){
		alarm = 1;
	}

	/*
	 * Status register would have been restored on exit of ISR,
	 * 	so clear LPM3_bits to continue operation instead of returning to sleep mode
	 */
	_bic_SR_register_on_exit(LPM3_bits);
}
