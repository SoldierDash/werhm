/*
 * flash.h
 *
 *  Created on: Mar 28, 2015
 *      Author: mdbalke
 */

#ifndef FLASH_H_
#define FLASH_H_

unsigned char flash_status_reg();
void flash_WREN();
void flash_WRSR(unsigned char value);

unsigned char flash_read(unsigned long addr);
void flash_read_buffer(unsigned long starting_addr, unsigned int num_bytes, unsigned char *buffer);
void flash_write(unsigned long addr, unsigned char value);
void wait_flash_busy();
void flash_reset();

unsigned char flash_setup();

#endif /* FLASH_H_ */
