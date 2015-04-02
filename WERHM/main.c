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

	volatile int rx_size = 0;
	volatile unsigned char status = 0;


	led_flash();

	//CC1101_strobe(CC_SFTX);
	//CC1101_strobe(CC_SFRX);

	while (1) {

		volatile unsigned char num_bytes = CC1101_read_status_register(CC_NUM_RXBYTES);

		CC1101_strobe(CC_SFRX);
		status = CC1101_strobe(CC_SRX);

 		while (!(P1IN & GDO2));

		cc1101_rcv_packet(rx, &rx_size);
		led_flash();


	}

}


