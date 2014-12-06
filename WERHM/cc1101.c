/*
 * cc1101.c
 *
 *  Created on: Dec 3, 2014
 *      Author: mdbalke
 */

#include <msp430.h>
#include "cc1101.h"


void cc1101_send_packet(unsigned char *data, int num_bytes){

	if(num_bytes <= 0 || num_bytes > 64){
		return;
	}


	CC1101_burst_reg_write(0x7F, data, num_bytes);

	CC1101_reg_write(0x3F, 0x03);

	CC1101_strobe(CC_STX);

	while(!GDO0_READ);
	while(GDO0_READ);

	//blink_green();

	CC1101_strobe(CC_SFTX);
}


/*
 *
 *
 *
 *	return: 0 if buffer successfully filled
 *			0xFF if unsuccessful
 *
 */
void cc1101_rcv_packet(unsigned char *data, int *num_bytes){

	int packet_length = 0;

	CC1101_strobe(CC_SRX);

	//while(!MISO_READ);
	//while(MISO_READ);

	if(cc1101_reg_read(CC_RXBYTES) & 0b01111111){ //If non-zero amnt of byte in FIFO

		packet_length = cc1101_reg_read(0xBF);
		*num_bytes = packet_length;

		data[0] = cc1101_reg_read(0xBF);

		CC1101_strobe(CC_SFRX);

		return 0;
	}else{
		return 0xFF;
	}
}




/*	Writes one byte to CC1101 register in single access mode (not burst)
 *
 * 	return: 0 if succesful write
 * 			0xFF if invalid address given
 *
 */
unsigned char CC1101_reg_write(unsigned char address, unsigned char data){

	//Check to make sure address is only 6 bits
	if((address & 0b11000000) != 0){
		return 0xFF;
	}

	unsigned char write_data[2];
	unsigned int status_bytes[2];

	write_data[0] = address;
	write_data[1] = data;

	spi_transaction(write_data, 2, status_bytes);

	//TODO: check status bytes to see if chip was not ready for write
	return 0;
}

/*
 * Reads ony byte from C1101 register in single access mode (not burst)
 *
 * return: 0xFF if invalid address given
 * 		   register value otherwise
 *
 */
unsigned char CC1101_reg_read(unsigned char address){
	//Check to make sure address is only 6 bits
	if((address & 0b11000000) != 0){
		return 0xFF;
	}

	unsigned char write_data[2];
	unsigned int read_data[2];

	write_data[0] = (0b10000000 | address);
	write_data[1] = 0;

	spi_transaction(write_data, 2, read_data);


	//TODO: check status byte in read_data[0] for errors

	return read_data[1];
}


/*
 * 	Send command strobe to CC1101
 *
 * 	return: 0 if successful
 * 			0xFF if invalid strobe
 *
 */
unsigned char CC1101_strobe(unsigned char strobe){

	if(strobe > 0x3D || strobe < 0x30){
		return 0xFF;
	}

	unsigned char write_data[1];
	unsigned int read_data[1];
	write_data[0] = strobe;

	spi_transaction(write_data, 1, read_data);

	return 0;

}

/*
 * 	Configure the cc1101
 *
 */
void cc1101_config(){

	// Write register settings
	    CC1101_reg_write(CC_IOCFG2,   0x0B); // GDO2 output pin config.
	    CC1101_reg_write(CC_IOCFG0,   0x06); // GDO0 output pin config. Edit: 0x06 -> 0x01 EoP alert
	    CC1101_reg_write(CC_PKTLEN,   0xFF); // Packet length.
	    CC1101_reg_write(CC_PKTCTRL1, 0x05); // Packet automation control.
	    CC1101_reg_write(CC_PKTCTRL0, 0x05); // Packet automation control.
	    CC1101_reg_write(CC_ADDR,     0x01); // Device address.
	    CC1101_reg_write(CC_CHANNR,   0x00); // Channel number.
	    CC1101_reg_write(CC_FSCTRL1,  0x0B); // Freq synthesizer control.
	    CC1101_reg_write(CC_FSCTRL0,  0x00); // Freq synthesizer control.
	    CC1101_reg_write(CC_FREQ2,    0x10); // Freq control word, high byte
	    CC1101_reg_write(CC_FREQ1,    0xA7); // Freq control word, mid byte.
	    CC1101_reg_write(CC_FREQ0,    0x62); // Freq control word, low byte.
	    CC1101_reg_write(CC_MDMCFG4,  0x2D); // Modem configuration.
	    CC1101_reg_write(CC_MDMCFG3,  0x3B); // Modem configuration.
	    CC1101_reg_write(CC_MDMCFG2,  0x73); // Modem configuration.
	    CC1101_reg_write(CC_MDMCFG1,  0x22); // Modem configuration.
	    CC1101_reg_write(CC_MDMCFG0,  0xF8); // Modem configuration.
	    CC1101_reg_write(CC_DEVIATN,  0x00); // Modem dev (when FSK mod en)
	    CC1101_reg_write(CC_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine
	    CC1101_reg_write(CC_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
	    CC1101_reg_write(CC_FOCCFG,   0x1D); // Freq Offset Compens. Config
	    CC1101_reg_write(CC_BSCFG,    0x1C); //  Bit synchronization config.
	    CC1101_reg_write(CC_AGCCTRL2, 0xC7); // AGC control.
	    CC1101_reg_write(CC_AGCCTRL1, 0x00); // AGC control.
	    CC1101_reg_write(CC_AGCCTRL0, 0xB2); // AGC control.
	    CC1101_reg_write(CC_FREND1,   0xB6); // Front end RX configuration.
	    CC1101_reg_write(CC_FREND0,   0x10); // Front end RX configuration.
	    CC1101_reg_write(CC_FSCAL3,   0xEA); // Frequency synthesizer cal.
	    CC1101_reg_write(CC_FSCAL2,   0x0A); // Frequency synthesizer cal.
	    CC1101_reg_write(CC_FSCAL1,   0x00); // Frequency synthesizer cal.
	    CC1101_reg_write(CC_FSCAL0,   0x11); // Frequency synthesizer cal.
	    CC1101_reg_write(CC_FSTEST,   0x59); // Frequency synthesizer cal.
	    CC1101_reg_write(CC_TEST2,    0x88); // Various test settings.
	    CC1101_reg_write(CC_TEST1,    0x31); // Various test settings.
	    CC1101_reg_write(CC_TEST0,    0x0B); // Various test settings.


	/*
	CC1101_reg_write(CC_FSCTRL1, 0x0B);
	CC1101_reg_write(CC_FSCTRL0, 0x00);
	CC1101_reg_write(CC_FREQ2, 0x10);
	CC1101_reg_write(CC_FREQ1, 0xA7);
	CC1101_reg_write(CC_FREQ0, 0x062);
	CC1101_reg_write(CC_MDMCFG4, 0x2D);
	CC1101_reg_write(CC_MDMCFG3, 0x3B);
	CC1101_reg_write(CC_MDMCFG2, 0x73);
	CC1101_reg_write(CC_MDMCFG1, 0x22);
	CC1101_reg_write(CC_MDMCFG0, 0xF8);
	CC1101_reg_write(CC_CHANNR, 0x00);
	CC1101_reg_write(CC_DEVIATN, 0x00);
	CC1101_reg_write(CC_FREND1, 0xB6);
	CC1101_reg_write(CC_FREND0, 0x10);
	CC1101_reg_write(CC_MCSM1, 0x3F); //Initially gone
	CC1101_reg_write(CC_MCSM0, 0x18);
	CC1101_reg_write(CC_FOCCFG, 0x1D);
	CC1101_reg_write(CC_BSCFG, 0x1C);
	CC1101_reg_write(CC_AGCCTRL2, 0xC7);
	CC1101_reg_write(CC_AGCCTRL1, 0x00);
	CC1101_reg_write(CC_AGCCTRL0, 0xB2);
	CC1101_reg_write(CC_FSCAL3, 0xEA);
	CC1101_reg_write(CC_FSCAL2, 0x0A);
	CC1101_reg_write(CC_FSCAL1, 0x00);
	CC1101_reg_write(CC_FSCAL0, 0x11);
	CC1101_reg_write(CC_FSTEST, 0x59);
	CC1101_reg_write(CC_TEST2, 0x88);
	CC1101_reg_write(CC_TEST1, 0x31);
	CC1101_reg_write(CC_TEST0, 0x0B);
	CC1101_reg_write(CC_IOCFG2, 0x0B);
	CC1101_reg_write(CC_IOCFG0, 0x06);
	CC1101_reg_write(CC_PKTCTRL1, 0x05); //0x04
	CC1101_reg_write(CC_PKTCTRL0, 0x05);
	CC1101_reg_write(CC_ADDR, 0x01); //0x00
	CC1101_reg_write(CC_PKTLEN, 0xFF);
	*/
}


unsigned char CC1101_burst_reg_write(unsigned char starting_address, unsigned char data[], int num_bytes){

	//Check to make sure address is only 6 bits
	if ((starting_address & 0b11000000) != 0) {
		return 0xFF;
	}

	unsigned char write_data[32];
	unsigned int status_bytes[32];

	write_data[0] = (0b01000000 | starting_address);
	//write_data[1] = 0x01;
	//write_data[2] = 0x02;
	//write_data[3] = 0x03;

	int i;
	for(i = 0; i < num_bytes; i++){
		write_data[i+1] = data[i];
	}

	spi_transaction(write_data, num_bytes + 1, status_bytes);

	//TODO: check status bytes to see if chip was not ready for write
	return 0;
}


/*
 * 	Performs an SPI transaction, sending bytes from data_write[]
 * 		along MOSI line while reading bytes from MISO and writing
 * 		them to data_read.
 *
 * 		Currently only implemented to work with one slave.
 * 			- Can be extended for multiple slaves by adding more CS lines
 * 				and passing selection as an argument.
 *
 */
void spi_transaction( unsigned char data_write[], int bytes, unsigned int *data_read) {
    int i, n, j;

    for(j = 0; j < bytes; j++){
    	data_read[j] = 0;
    }

    //Disable global interrupts
    _BIC_SR(GIE);

    // Set Chip Select low, wait for chip to be ready
    P1OUT &= ~CS;
    while(MISO_READ != 0);

    for(n = 0; n < bytes; n++ ) {
        for(i = 7; i >= 0; i-- ) {

            // Send data on MOSI line, most significant bit first.
            if( data_write[n] & (1 << i) ) {
                P1OUT |= MOSI;
            } else {
                P1OUT &= ~MOSI;
            }

            // Pulse SCK
            P1OUT &= ~SCK;
            __delay_cycles(500);
            P1OUT |= SCK;
            __delay_cycles(500);


            // Read bit from MISO
            if(MISO_READ){
            	data_read[n] |= (1 << i);
            }

        }
        _delay_cycles(1000);
    }

    // CS high to end transaction
    P1OUT |= CS;
    //blink_red();

    //Re-enable global interrupts
    _BIS_SR(GIE);
}


