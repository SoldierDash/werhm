#include <msp430.h>
#include "spi.h"
#include "cc1101.h"

#define CS_ENABLE (P1OUT &= ~CS)
#define CS_DISABLE (P1OUT |= CS)

#define HEADER_READ(x) (x | CC_HEADER_RW)
#define HEADER_WRITE(x) (x & ~CC_HEADER_RW)
#define HEADER_BURST(x) (x | CC_HEADER_BURST)
#define HEADER_SINGLE(x) (x & ~CC_HEADER_BURST)

unsigned char CC1101_reg_write(unsigned char address, unsigned char data) {
	unsigned char header = (address & ~CC_HEADER_RW) & ~CC_HEADER_BURST;

	volatile unsigned char status;

	CS_ENABLE;

	//TODO Wait for CC1101 to be ready
	spi_tx(header);
	status = spi_tx(data);

	CS_DISABLE;

	return status;
}

unsigned char CC1101_burst_reg_write(unsigned char starting_address, unsigned char *data, int num_bytes) {
	unsigned char header = (starting_address & ~CC_HEADER_RW) | CC_HEADER_BURST;

	volatile unsigned char status;
	int i;

	CS_ENABLE;
	spi_tx(header);
	for(i = 0; i < num_bytes; i++)
		status = spi_tx(data[i]);
	CS_DISABLE;

	return status;
}

unsigned char CC1101_reg_read(unsigned char address) {
	unsigned char header = (address | CC_HEADER_RW) & ~CC_HEADER_BURST;

	volatile unsigned char status;

	CS_ENABLE;

	spi_tx(header);
	status = spi_tx(0x00);

	CS_DISABLE;

	return status;
}

unsigned char CC1101_burst_reg_read (unsigned char starting_address, unsigned char *data, int num_bytes) {
	unsigned char header = (starting_address | CC_HEADER_RW) | CC_HEADER_BURST;

	volatile unsigned char status;
	int i;


	CS_ENABLE;
	status = spi_tx(header);
	for(i = 0; i < num_bytes; i++)
		data[i] = spi_tx(0x00);
	CS_DISABLE;

	return status;
}

unsigned char CC1101_strobe(unsigned char strobe) {
	volatile unsigned char status;

	CS_ENABLE;

	status = spi_tx(strobe);

	CS_DISABLE;

	return status;
}

void cc1101_config() {
	P1DIR |= CS;
	P1OUT |= CS;
}

void cc1101_send_packet(unsigned char *data, int num_bytes) {
	CC1101_burst_reg_write(0x3F, data, num_bytes);
	CC1101_strobe(CC_STX);
}

void cc1101_rcv_packet(unsigned char *data, int *num_bytes) {
	*num_bytes = CC1101_reg_read(CC_RXBYTES & CC_NUM_RXBYTES);
	CC1101_burst_reg_read(0x3F, data, *num_bytes);
}
