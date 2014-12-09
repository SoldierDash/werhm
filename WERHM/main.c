/*
 * main.c
 *
 *  Created on: Dec 1, 2014
 *      Author: mdbalke
 */

#include <msp430.h>
#include "spi.h"
#include "cc1101.h"
#include "microcontroller.h"





int main(void) {


	//WDTCTL = WDTPW + WDTHOLD; Handled in mcu_setup()

	mcu_setup();

	spi_setup();

	cc1101_config(0x01, 0x00); // Device address 1, Channel number 0



	volatile unsigned char var2[64];
	volatile unsigned char var2_size;

	int i;
	for(i = 0; i < 64; i++){
		var2[i] = 0;
	}

	//cc1101_rcv_packet(var2, &var2_size);
	//cc1101_rcv_packet(var2, &var2_size);

	// Wait for GDO2 to go high indicating RX buffer exceeds threshold
	while(P1IN & GDO2);

	cc1101_rcv_packet(var2, &var2_size);


	/* TX
	volatile unsigned char var[32];
	var[0] = sizeof(var) - 1;
	var[1] = 0x01;
	int i;
	for(i = 2; i < 32; i++)
		var[i] = i - 1;

	cc1101_send_packet(var, sizeof(var));

	*/


	//Reset chip to restore default register values
	//CC1101_strobe(CC_SRES);

	//cc1101_config();

	//blink_red();

	//unsigned char packet[3];

	//packet[0] = 0x02;
	//packet[1] = 0x05;
	//packet[2] = 0x03;

	//cc1101_send_packet(packet, 3);

	blink_red();

	while(1);
}




