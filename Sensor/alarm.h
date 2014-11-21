/*
 * alarm.h
 *
 *  Created on: Nov 17, 2014
 *      Author: mdbalke
 */

#ifndef ALARM_H_
#define ALARM_H_

#define ALARM_SECONDS 1
#define ALARM_MINUTES 0
#define ALARM_HOURS 0

volatile int alarm = 0;
volatile int seconds, minutes, hours;

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMERA0_ISR() {

	seconds++;
	if (seconds >= 60) {
		seconds = 0;
		minutes++;
		if (minutes >= 60) {
			minutes = 0;
			hours++;
		}
	}

	if (seconds == ALARM_SECONDS && minutes == ALARM_MINUTES
			&& hours == ALARM_HOURS) {
		seconds = 0;
		minutes = 0;
		hours = 0;
		alarm = 1;
	}
	//Clear status register on exit of ISR
	_bic_SR_register_on_exit (LPM3_bits);
}

#endif /* ALARM_H_ */
