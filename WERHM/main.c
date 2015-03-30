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
#include "cc1101.h"

void main(){



	mcu_setup();
	spi_setup();

	cc1101_config(1, 0);


	int i;
	int tx_size = 32;
	tx[0] = tx_size;
	tx[1] = 0x01;
	for(i = 2; i < tx_size; i++){
		tx[i] = i;
	}


	volatile unsigned char status_reg = CC1101_reg_read(CC_IOCFG0);

	led_flash();
	cc1101_send_packet(tx, tx_size);

	led_flash();
	while(1);
}
