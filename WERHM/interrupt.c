/*
 * interrupt.c
 *
 *  Created on: Oct 25, 2014
 *      Author: Brandon
 */

#include "microcontroller.h"
#include <msp430.h>

#pragma vector=PRAGMA_VECTOR
__interrupt void Port_1_ISR(void) {

	clearInterruptPins(0x08); //Clear P1.3


	flashLED();

	//_\bic_SR_register_on_exit(LPM4_bits);
}
