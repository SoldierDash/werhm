/*
 * microcontroller.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */

#ifndef _MICROCONTROLLER_H_
#define _MICROCONTROLLER_H_

#define LED_PIN BIT3
#define LED_ON P1OUT |= LED_PIN
#define LED_OFF P1OUT &= ~LED_PIN

#define FLASH_ON P2OUT |= BIT3;

void mcu_setup();

/* Sleep mode with interrupt wakeup */
void mcu_sleep_gie();

/* Wait for interrupt wakeup */
void mcu_wait_gie();

void led_flash();


#endif /* MICROCONTROLLER_H_ */
