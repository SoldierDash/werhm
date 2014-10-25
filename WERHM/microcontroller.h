/*
 * microcontroller.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */

#ifndef _MICROCONTROLLER_H_
#define _MICROCONTROLLER_H_

#define PRAGMA_VECTOR PORT1_VECTOR

void disableWatchdog();

void sleepMode();

void setOutputPins(int pins);

void setInputPins(int pins);

/**
 * Enables interrupt on specified pins
 * Sets the flag for each pin to 0
 */
void setInterruptPins(int pins);

void clearInterruptPins(int pins);

void setFrequency();

void flashLED();


#endif /* MICROCONTROLLER_H_ */
