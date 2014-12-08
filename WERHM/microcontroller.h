/*
 * microcontroller.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */

#ifndef _MICROCONTROLLER_H_
#define _MICROCONTROLLER_H_

#define PRAGMA_VECTOR PORT1_VECTOR

volatile unsigned char rx_flag;

void mcu_setup();

/* Sleep mode with interrupt wakeup */
void mcu_sleep_gie();

/* Wait for interrupt wakeup */
void mcu_wait_gie();

/* LED debugers, uses LaunchPad pins Red == P1.0 and Green == P1.6 */
void led_flash();
void blink_red();
void blink_green();


#endif /* MICROCONTROLLER_H_ */
