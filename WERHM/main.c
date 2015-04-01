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


	unsigned char TX = 0;

	mcu_setup();
	spi_setup();
	cc1101_config(1, 0);

	int i;
	int tx_size = 64;
	tx[0] = tx_size;
	tx[1] = 0x01;
	for(i = 2; i < tx_size; i++){
		tx[i] = i;
	}

	if(TX){
		//send packet
		cc1101_send_packet(tx, tx_size);
		led_flash();

		while(1);
	}else{
		//receive packet

		CC1101_strobe(CC_SRX);

		while(!(P1IN & GDO2));
		int rx_size = 0;
		cc1101_rcv_packet(rx, &rx_size);
		led_flash();

	}




	//led_flash();
	//cc1101_send_packet(tx, tx_size);

	led_flash();
	while(1);
}
