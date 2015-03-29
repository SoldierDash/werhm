/*
 * main.c
 * Main loop for WERHM node
 *
 *  Created on: Mar 28, 2015
 *      Author: Mitch
 */


#include <msp430.h>
#include "microcontroller.h"
#include "spi.h"

void main(){

	volatile unsigned char status;

	mcu_setup();
	spi_setup();

	_delay_cycles(100000);

	FLASH_SELECT;
	_delay_cycles(1000);

	spi_tx_lpm_iu(0x05);
	status = spi_tx_lpm_iu(0);
	FLASH_DESELECT;

	led_flash();
	while(1);
}
