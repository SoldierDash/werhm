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

	cc1101_config();



	_delay_cycles(1000);

	CC1101_strobe(CC_SRES);

	_delay_cycles(1000);


	volatile unsigned char val1 = 0;
	val1 = CC1101_reg_read(0x00);

	/* RX

	volatile unsigned char pkt_length = 0;
	volatile unsigned char val1 = 0;
	volatile unsigned char val2 = 0;
	volatile unsigned char val3 = 0;

	P1OUT &= ~CS;
	spi_tx_am(CC_SRX); //RX mode
	P1OUT |= CS;

	_delay_cycles(100000);

	while(1){

		P1OUT &= ~CS;
		spi_tx_am(CC_RXBYTES);
		val3 = spi_tx_am(0) & 0b01111111;
		P1OUT |= CS;

		//check there are bytes in buffer
		if(val3 != 0){
			P1OUT &= ~CS;
			val1 = spi_tx(0xBF);
			val2 = spi_tx(0);
			P1OUT |= CS;
			break;
		}
		_delay_cycles(1000);
	}

	P1OUT |= CS;

	*/

	/* TX

	volatile unsigned char val1 = 10;
	volatile unsigned char val2[64];
	volatile unsigned char val3;
	volatile unsigned char val4;
	volatile char i = 0;

	P1OUT &= ~CS;

	// Burst tx
	val1 = spi_tx_am(0x7f);

	for(i = 0; i < 64; i++)
		val2[i] = spi_tx_am(i);

	P1OUT |= CS;

	_delay_cycles(1000);

	P1OUT &= ~CS;

	val3 = spi_tx_am(CC_STX);

	P1OUT |= CS;

	_delay_cycles(1000);

	P1OUT &= ~CS;

	val4 = spi_tx_am(CC_SNOP);

	P1OUT |= CS;

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




