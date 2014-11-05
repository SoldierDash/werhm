/*
 * microcontroller.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Brandon
 */

#ifndef _MICROCONTROLLER_H_
#define _MICROCONTROLLER_H_

#define PRAGMA_VECTOR PORT1_VECTOR

void mcu_setup();

/* Sleep mode with interrupt wakeup */
void mcu_sleep_gie();

/* Wait for interrupt wakeup */
void mcu_wait_gie();

void led_flash();


#endif /* MICROCONTROLLER_H_ */
