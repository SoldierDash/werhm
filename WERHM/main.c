/*
 * main.c
 *
 *  Created on: Feb 17, 2015
 *      Author: mdbalke
 */

#include <msp430.h>
#include "microcontroller.h"

#include "spi.h"

#define CS_LOW P1OUT &= ~CS
#define CS_HIGH P1OUT |= CS

#define delay _delay_cycles(4000);

volatile char TXData;

unsigned char MST_Data, SLV_Data;

unsigned char read_mem(unsigned char address);
void mem_write(unsigned char address, unsigned char value);
void mem_reset();
void mem_WREN();

unsigned char status_register();

int main(void) {
	volatile unsigned int i;

	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	P1DIR |= WP + HOLD + CS + BIT7;                            //

	P1OUT |= CS + HOLD + WP;
	P1REN |= CS + HOLD + WP;

	P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
	UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	UCA0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt



	mem_reset();

	mem_WREN();
	delay;
	mem_write(0, 0x17);
	delay;
	volatile unsigned char mem_val = read_mem(0);
	delay;
	volatile unsigned char status = status_register();

	P1OUT |= BIT7;
	while (1);
}

void mem_write(unsigned char address, unsigned char value) {
	P1OUT &= ~CS;
	spi_send(0x02);
	spi_send(0x00);
	spi_send(0x00);
	spi_send(0x06);
	spi_send(value);
	P1OUT |= CS;
}

unsigned char read_mem(unsigned char address) {

	unsigned char out = 0;

	P1OUT &= ~CS;
	spi_send(0x03);
	spi_send(0x00);
	spi_send(0x00);
	spi_send(0x06);
	out = spi_send(0x00);
	P1OUT |= CS;
	return out;
}

unsigned char status_register() {

	unsigned char out = 0;
	P1OUT &= ~CS;
	spi_send(0x05);
	out = spi_send(0x00);
	P1OUT |= CS;
	return out;

}

void mem_reset() {
	P1OUT &= ~CS;
	spi_send(0x60);
	P1OUT |= CS;

}

void mem_WREN() {

	P1OUT &= ~CS;
	spi_send(0x50);
	P1OUT |= CS;

	P1OUT &= ~CS;
	spi_send(0x01);
	spi_send(0x00);
	P1OUT |= CS;

	P1OUT &= ~CS;
	spi_send(0x06);
	P1OUT |= CS;

}
