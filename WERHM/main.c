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

#define TX_RX 1



int main(void) {


	//WDTCTL = WDTPW + WDTHOLD; Handled in mcu_setup()

	mcu_setup();

	spi_setup();

	cc1101_config(0x01, 0x00); // Device address 1, Channel number 0

	unsigned char rx[64];

	volatile int rx_packets;
	volatile int CRC_pass;

	if(TX_RX){

		//Continuously send packets
		unsigned char var[32];
		var[0] = sizeof(var) - 1;
		var[1] = 0x01;
		int i;


	volatile unsigned char var2[64];
	volatile unsigned char var2_size;

		while(1){

			//TODO
			//Init TX timer as timerA
			//Check TX flag instead of delay_cycles()

			CC1101_send(var, sizeof(var));
			blink_red();
			_delay_cycles(1500000);
		}
	}else{

		//Receive packet
		int rx_size = 0;
		unsigned char status;

		rx_packets = 0;
		CRC_pass = 0;
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

<<<<<<< HEAD
			rx_size = 0;
			status = cc1101_rcv_packet(rx, &rx_size);
			rx_packets++;

			//Check CRC-OK bit
			if(status == 0){
				//CRC pass
				CRC_pass++;
				blink_green();
			}else{
				//Packet received but CRC failed
				blink_red();
			}
			//CRC_percent = (float) (CRC_pass / rx_packets) * 100;
=======
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

	//blink_red();

	//while(1);
}


