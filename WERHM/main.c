/*
 * main.c
 *
 *  Created on: Feb 17, 2015
 *      Author: mdbalke
 */

#include <msp430.h>
#include "microcontroller.h"
#include "interrupt.h"

#include "spi.h"

#define CS BIT0
#define CS_HIGH (P1OUT |= CS)
#define CS_LOW (P1OUT &= ~CS)
#define delay _delay_cycles(1000)

#define WREN 0x06


int main(void) {

	mcu_setup();
	spi_setup();

	P1DIR |= CS;

	volatile unsigned char read_value = 0xFF;

	CS_HIGH;
	delay;

	CS_LOW;
	spi_tx_am(0x03);
	read_value = spi_tx_am(0x00);
	CS_HIGH;
	while(1);




	return 0;
}
