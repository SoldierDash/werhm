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
#include "flash.h"

void main(){

	volatile unsigned char status, status2;
	volatile unsigned char read1, read2;

	mcu_setup();
	spi_setup();

	_delay_cycles(100000);

	status = flash_setup();


	led_flash();
	while(1);
}
