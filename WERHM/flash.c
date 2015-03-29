/*
 * flash.c
 *
 *  Created on: Mar 28, 2015
 *      Author: mdbalke
 */

#include <msp430.h>
#include "microcontroller.h"
#include "spi.h"
#include "flash.h"

/*
 * Reads the value of the SST25VF040B status register
 *
 */
unsigned char flash_status_reg() {

	unsigned char out = 0;
	FLASH_SELECT;
	_delay_cycles(100);

	spi_tx_am(0x05);
	out = spi_tx_am(0);
	FLASH_DESELECT;
	return out;
}

/*
 * Enables the write-enable latch on the SST25VF040B
 *
 */
void flash_WREN() {

	FLASH_SELECT;
	spi_tx_am(0x06);
	FLASH_DESELECT;
}

/*
 * Writes to the SST25VF040B status register
 *
 */
void flash_WRSR(unsigned char value) {

	flash_WREN();
	FLASH_SELECT;
	spi_tx_am(0x01);
	spi_tx_am(value);
	FLASH_DESELECT;
}

/*
 * Reads one byte stored in memory
 *
 */
unsigned char flash_read(unsigned long addr) {

	unsigned char out = 0;

	FLASH_SELECT;
	spi_tx_am(0x03);
	spi_tx_am((addr & 0xFFFFFF) >> 16);
	spi_tx_am((addr & 0xFFFF) >> 8);
	spi_tx_am(addr & 0xFF);
	out = spi_tx_am(0);
	FLASH_DESELECT;
	return out;
}

/*
 * Reads multiple bytes from flash memory
 *    - a maximum of 128 bytes can be read at a time
 * TODO: consider writing bytes to global buffer instead of passing pointer
 *
 */
void flash_read_buffer(unsigned long starting_addr, unsigned int num_bytes,
		unsigned char *buffer) {

	unsigned int i;

	FLASH_SELECT;
	spi_tx_am(0x03);
	spi_tx_am((addr & 0xFFFFFF) >> 16);
	spi_tx_am((addr & 0xFFFF) >> 8);
	spi_tx_am(addr & 0xFF);
	for(i = 0; i < num_bytes; i++){
		buffer[i] = spi_tx_am(0);
	}
}

/*
 * Writes one byte to memory
 *
 * Note: device address must have been erased for write operation to take place
 */
void flash_write(unsigned long addr, unsigned char value) {

	flash_WREN();

	FLASH_SELECT;
	spi_tx_am(0x02);
	spi_tx_am((addr & 0xFFFFFF) >> 16);
	spi_tx_am((addr & 0xFFFF) >> 8);
	spi_tx_am(addr & 0xFF);
	spi_tx_am(value);
	FLASH_DESELECT;
	wait_flash_busy();
}

/*
 * Waits for any write operation to finish
 *
 * TODO: add detection for infinite wait in case device dies
 *
 */
void wait_flash_busy() {
	while ((flash_status_reg() & 0x01) == 0x01) {
		//_delay_cycles(100);
	}
}

/*
 * Resets whole chip
 *
 */
void flash_reset() {

	flash_WREN();
	FLASH_SELECT;
	spi_tx_am(0x60);
	FLASH_DESELECT;
	wait_flash_busy();
}

/*
 * Resets SST25VF040B and removes all block write-protection
 *
 */
unsigned char flash_setup() {

	flash_reset();
	flash_WRSR(0);
	return flash_status_reg();
}
