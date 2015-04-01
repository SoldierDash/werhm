#include <msp430.h>
#include "spi.h"
#include "cc1101.h"
#include "microcontroller.h"

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

unsigned char CC1101_burst_reg_write(unsigned char starting_address,
		unsigned char *data, int num_bytes) {
	unsigned char header = (starting_address & ~CC_HEADER_RW) | CC_HEADER_BURST;

	volatile unsigned char status;
	int i;

	CS_ENABLE;
	spi_tx(header);
	for (i = 0; i < num_bytes; i++)
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

unsigned char CC1101_burst_reg_read(unsigned char starting_address,
		unsigned char *data, int num_bytes) {
	unsigned char header = (starting_address | CC_HEADER_RW) | CC_HEADER_BURST;

	volatile unsigned char status;
	int i;

	CS_ENABLE;
	status = spi_tx(header);
	for (i = 0; i < num_bytes; i++)
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

void cc1101_config(unsigned char device_address, unsigned char channel_number) {
	/* Set Slave Select as output and set high */
	P1DIR |= CS;	// Slave select output
	P1OUT |= CS;	// Slave select high
	P1DIR &= ~GDO0;	// GDO0 input
	P1DIR &= ~GDO2;	// GDO2 input

	_delay_cycles(1000);

	CC1101_strobe(CC_SRES);

	_delay_cycles(1000);

	// Write register settings
	CC1101_reg_write(CC_IOCFG2, 0x01); // GDO2 output pin config.			// GDO2 High when TX, and low when finished
	CC1101_reg_write(CC_IOCFG0, 0x06); // GDO0 output pin config.			// RX Threshold trigger on GD0
	CC1101_reg_write(CC_PKTLEN, 16); // Packet length.
	CC1101_reg_write(CC_PKTCTRL1, 0x05); // Packet automation control.
	CC1101_reg_write(CC_PKTCTRL0, 0x05); // Packet automation control.		// CRC enabled and Variable packet length enabled, data whitening off
	CC1101_reg_write(CC_ADDR, device_address); // Device address.					// Device address for packet filtering
	CC1101_reg_write(CC_CHANNR, channel_number); // Channel number.					// Shifts comm freq by (Channel spacing) * ~100kHz
	CC1101_reg_write(CC_FSCTRL1, 0x0B); // Freq synthesizer control.
	CC1101_reg_write(CC_FSCTRL0, 0x00); // Freq synthesizer control.
	CC1101_reg_write(CC_FREQ2, 0x10); // Freq control word, high byte
	CC1101_reg_write(CC_FREQ1, 0xA7); // Freq control word, mid byte.
	CC1101_reg_write(CC_FREQ0, 0x62); // Freq control word, low byte.
	CC1101_reg_write(CC_MDMCFG4, 0x2D); // Modem configuration.
	CC1101_reg_write(CC_MDMCFG3, 0x3B); // Modem configuration.
	CC1101_reg_write(CC_MDMCFG2, 0x73); // Modem configuration.
	CC1101_reg_write(CC_MDMCFG1, 0x22); // Modem configuration.
	CC1101_reg_write(CC_MDMCFG0, 0xF8); // Modem configuration.
	CC1101_reg_write(CC_DEVIATN, 0x00); // Modem dev (when FSK mod en)
	CC1101_reg_write(CC_MCSM1, 0x3F); // MainRadio Cntrl State Machine
	CC1101_reg_write(CC_MCSM0, 0x18); // MainRadio Cntrl State Machine
	CC1101_reg_write(CC_FOCCFG, 0x1D); // Freq Offset Compens. Config
	CC1101_reg_write(CC_BSCFG, 0x1C); //  Bit synchronization config.
	CC1101_reg_write(CC_AGCCTRL2, 0xC7); // AGC control.
	CC1101_reg_write(CC_AGCCTRL1, 0x00); // AGC control.
	CC1101_reg_write(CC_AGCCTRL0, 0xB2); // AGC control.
	CC1101_reg_write(CC_FREND1, 0xB6); // Front end RX configuration.
	CC1101_reg_write(CC_FREND0, 0x10); // Front end RX configuration.
	CC1101_reg_write(CC_FSCAL3, 0xEA); // Frequency synthesizer cal.
	CC1101_reg_write(CC_FSCAL2, 0x0A); // Frequency synthesizer cal.
	CC1101_reg_write(CC_FSCAL1, 0x00); // Frequency synthesizer cal.
	CC1101_reg_write(CC_FSCAL0, 0x11); // Frequency synthesizer cal.
	CC1101_reg_write(CC_FSTEST, 0x59); // Frequency synthesizer cal.
	CC1101_reg_write(CC_TEST2, 0x88); // Various test settings.
	CC1101_reg_write(CC_TEST1, 0x31); // Various test settings.
	CC1101_reg_write(CC_TEST0, 0x0B); // Various test settings.
	CC1101_reg_write(CC_FIFOTHR, 0x03); // RX/TX FIFO capacity trigger		// 0x00 == 4 bytes

	//TODO Device checking
	//CC1101_strobe(CC_SFRX);

}

void cc1101_send_packet(unsigned char *data, int num_bytes) {

	//TODO Handle num_bytes > MAX_TXFIFO - 2 (for length and device addr)

	//TODO Handle packet size and device address as first two bytes
	/*
	 * CC1101_reg_write(0x3F, length)
	 * CC1101_reg_write(0x3F, dev addr)
	 */
	CC1101_strobe(CC_SFTX);

	CC1101_burst_reg_write(0x3F, data, num_bytes);
	CC1101_strobe(CC_STX);

	while (!(P1IN & GDO0));
	while (P1IN & GDO0);
}

/*
 *	Sends num_bytes from global tx_buffer.  Data should be written to buffer before calling this function.
 *	If an ACK is received, it is stored in global rx_buffer
 *
 */
unsigned char cc1101_send(int num_bytes) {

	int i;
	int ACK_size = 0;
	int ACK_received = 0;

	cc1101_send_packet(buffer, num_bytes);
	blink_red();

	TACCTL0 = CCIE;						// CCR0 interrupt enabled
	TACTL = TASSEL_2 + MC_1 + ID_3;		// SMCLK/8, upmode
	TACCR0 = 10000;					// 12.5 Hz

	/*
	for (i = 0; i < 32; i++) {
		ACK[i] = i;
	}
	*/

	CC1101_strobe(CC_SRX);

	//Wait for ACK or timeout
	while ((!(P1IN & GDO2)) && timer_flag == 0);

	//Stop timerA
	TACCR0 = 0;

	if (timer_flag == 1) {

		ACK_received = 1;

		blink_red();
		blink_red();
		blink_red();
	} else {
		//cc1101_rcv_packet(ACK, &ACK_size);
		blink_green();
	}

	timer_flag = 0;

	return ACK_received;
}



/*
 * Reads buffer from RX_FIFO after checking that there is data to be read
 *
 * return: 0xFF if RX_FIFO overflow
 * 		   0 if data was read from buffer and CRC-pass
 * 		   1 if data was read from buffer but CRC-fail
 */
unsigned char cc1101_rcv_packet(unsigned char *data, int *num_bytes) {

	unsigned char status[2];
	char pktLen;

	if ((CC1101_read_status_register(CC_RXBYTES) & CC_NUM_RXBYTES) != 0) {
		pktLen = CC1101_reg_read(0xBF); // Read length byte
		*num_bytes = (CC1101_read_status_register(CC_RXBYTES) & CC_NUM_RXBYTES);

		if (pktLen <= *num_bytes) {
			CC1101_burst_reg_read(0xFF, data, pktLen); // Pull data

			// Read appended status bytes
			CC1101_burst_reg_read(0xFF, status, 2);

			//return CRC check
			if ((status[1] & 0x08) != 0) {
				return 0;
			} else {
				return 1;
			}
		} else {
			// Return the large size
			*num_bytes = pktLen;

			// Flush RXFIFO
			CC1101_strobe(CC_SFRX);

			// Error
			return 0xFF;
		}
	} else
		// Error
		return 0xFF;
}

/*
 * Send CC1101 into sleep mode with wake-on-radio enabled
 */
unsigned char CC1101_sleep_wake_on_radio() {

	//Init WOR

	//Start WOR
	return 0;
}

/*
 * Reads the value of a read-only status register
 *
 */
unsigned char CC1101_read_status_register(unsigned char address) {

	unsigned char header = address | CC_HEADER_RW | CC_HEADER_BURST;

	volatile unsigned char data;

	CS_ENABLE;

	spi_tx(header);
	data = spi_tx(0x00);

	CS_DISABLE;

	return data;

}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt
void Timer_A(void) {
	static int timerA_count = 0;

	timerA_count++;
	if (timerA_count >= 30) {
		timerA_count = 0;
		timer_flag = 1;
	}

}

/*
 * Calculates 8-bit checksum from buffer in
 *
 * in must be filled and allocated, length does NOT include the last byte for checksum (can be changed)
 *
 * return: 8-bit checksum
 *
 */
unsigned char generate_checksum(unsigned char *in, int length) {

	int i;
	unsigned char sum = 0;

	for (i = 0; i < length; i++) {
		sum += (in[i] * i);
	}

	return ~sum;
}

/*
 *	Checks buffer for errors by recalculating checksum against the last byte of the buffer
 *
 *	length DOES include the last byte of the buffer for the checksum (can be changed)
 *
 *	return: 1 if no error detected
 *			0 of error detected and checksum failed
 *
 */
unsigned char check_checksum(unsigned char *in, int length) {

	int i;
	unsigned char sum = 0;
	for (i = 0; i < length - 1; i++) {
		sum += (in[i] * i);
	}

	if ((sum & in[length - 1]) == 0) {
		return 1;
	} else {
		return 0;
	}
}

