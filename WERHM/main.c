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

//unsigned char buffer[32];
//unsigned char ACK[32];


int main(void) {

	//WDTCTL = WDTPW + WDTHOLD; Handled in mcu_setup()

	mcu_setup();
	spi_setup();

	cc1101_config(0x01, 0x00); // Device address 1, Channel number 0


	if (TX_RX) {

		//Continuously send packets
		buffer[0] = sizeof(buffer) - 1;
		buffer[1] = 0x01;
		int i;

		buffer[0] = sizeof(buffer) - 1;
		buffer[1] = 0x01;
		for (i = 2; i < 32; i++) {
			buffer[i] = i - 1;
		}


		for (i = 0; i < 32; i++) {
			ACK[i] = i;
		}

		while (1) {

			//cc1101_send(var, sizeof(var));

			cc1101_send(sizeof(buffer));

			//TODO
			//Init TX timer as timerA
			//Check TX flag instead of delay_cycles()

			/*
			cc1101_send_packet(buffer, sizeof(buffer));
			blink_red();

			TACCTL0 = CCIE;						// CCR0 interrupt enabled
			TACTL = TASSEL_2 + MC_1 + ID_3;		// SMCLK/8, upmode
			TACCR0 =  10000;					// 12.5 Hz

			//Wait for ACK
			CC1101_strobe(CC_SRX);

			for (i = 0; i < 32; i++) {
				ACK[i] = i;
			}

			//while(!(P1IN & GDO2));
			//cc1101_rcv_packet(ACK, &ACK_size);
			//blink_green();


			while ((!(P1IN & GDO2)) && timer_flag == 0);

			TACCR0 = 0;
			if (timer_flag == 1) {
				blink_red();
				blink_red();
				blink_red();
			} else {
				cc1101_rcv_packet(ACK, &ACK_size);
				blink_green();
			}

			timer_flag = 0;

			*/
			_delay_cycles(1500000);
		}
	} else {

		//Receive packets

		int rx_size = 0;
		int i;

		CC1101_strobe(CC_SRX);

		// Wait for GDO2 to go high indicating RX buffer exceeds threshold
		while (1) {

			ACK[0] = sizeof(ACK) - 1;
			ACK[1] = 0x01;
			for (i = 2; i < 32; i++) {
				ACK[i] = i;
			}


			while(!(P1IN & GDO2));

			rx_size = 0;
			cc1101_rcv_packet(buffer, &rx_size);
			blink_red();

			_delay_cycles(200000);

			cc1101_send_packet(ACK, sizeof(ACK));

			blink_green();

			CC1101_strobe(CC_SRX);
		}
	}

	//while(1);
}


