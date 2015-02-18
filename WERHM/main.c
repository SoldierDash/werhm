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

#define TX_RX 0

//unsigned char buffer[32];
//unsigned char ACK[32];

#define tx_size 16


int main(void) {

	//WDTCTL = WDTPW + WDTHOLD; Handled in mcu_setup()

	mcu_setup();
	spi_setup();

	cc1101_config(0x01, 0x00); // Device address 1, Channel number 0


	if (TX_RX) {

		//Continuously send packets
		int i;

		buffer[0] = tx_size;
		buffer[1] = 0x01;
		for (i = 2; i < tx_size; i++) {
			buffer[i] = i - 1;
		}

		buffer[tx_size-1] = generate_checksum(buffer, tx_size-2);
		//buffer[31] = 12;

		for (i = 0; i < tx_size; i++) {
			ACK[i] = i;
		}

		while (1) {

			cc1101_send(tx_size);

			_delay_cycles(1500000);
		}
	} else {

		//Receive packets

		volatile int rx_size = 0;
		int i;

		CC1101_strobe(CC_SRX);

		// Wait for GDO2 to go high indicating RX buffer exceeds threshold
		while (1) {

			ACK[0] = sizeof(ACK) - 1;
			ACK[1] = 0x01;
			for (i = 2; i < tx_size; i++) {
				ACK[i] = i;
			}


			while(!(P1IN & GDO2));

			rx_size = 0;
			cc1101_rcv_packet(buffer, &rx_size);
			blink_red();

			if(check_checksum(buffer, rx_size) == 1){

				cc1101_send_packet(ACK, tx_size);
				blink_green();
			}else{
				blink_red();
				blink_red();
				blink_red();
			}

			CC1101_strobe(CC_SRX);
		}
	}

	//while(1);
}


