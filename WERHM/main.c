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

void main() {

	mcu_setup();
	spi_setup();
	cc1101_config(1, 0);

	int i;
	int tx_size = 64;
	tx[0] = tx_size;
	tx[1] = 0x01;
	for (i = 2; i < tx_size; i++) {
		tx[i] = i;
	}

	volatile int rx_size = 0;
	volatile unsigned char status, status2;

	led_flash();

	while (1) {


		CC1101_strobe(CC_SRX);

		status = CC1101_read_status_register(CC_MARCSTATE);
		while(CC1101_read_status_register(CC_MARCSTATE) == 0x08);
		status2 = CC1101_read_status_register(CC_MARCSTATE);

 		while (!(P1IN & GDO2));

		int temp = 0;
		cc1101_rcv_packet(rx, &temp);
		rx_size = temp;
		led_flash();

		//status = CC1101_read_status_register(CC_MARCSTATE);



	}

}


