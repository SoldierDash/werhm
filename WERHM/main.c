/*
 * main.c
 *
 *  Created on: Dec 1, 2014
 *      Author: mdbalke
 */

#include <msp430.h>
#include "cc1101.h"
#include "spi.h"
#include "microcontroller.h"


void blink_red();
void blink_green();


int main(void) {


	WDTCTL = WDTPW + WDTHOLD;

	mcu_setup();

	spi_setup();



	//P1DIR |= BIT0 + BIT6;
	//P1DIR |= (CS + MOSI + SCK);
	//P1DIR &= ~(MISO + GDO0);
	//P1DIR |= CS + BIT0;
	P1DIR |= CS;

	P1OUT |= CS;
	_delay_cycles(1000);



	volatile unsigned int val1 = 10;
	volatile unsigned int val2 = 10;

	val1 = spi_tx_am(0b10000000);
	val2 = spi_tx_am(0);

	P1OUT &= ~CS;


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


void blink_red(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT0;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT0;
}

void blink_green(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT6;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT6;
}

