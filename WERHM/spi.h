/*
 * SPI interface
 *
 *  Created on: Nov 5, 2014
 *      Author: Brandon
 */

#ifndef SPI_H_
#define SPI_H_

#define CC1101_CS BIT5
#define CC1101_SELECT P1OUT &= ~CC1101_CS
#define CC1101_DESELECT P1OUT |= CC1101_CS

#define FLASH_CS BIT2
#define FLASH_SELECT P1OUT &= ~FLASH_CS
#define FLASH_DESELECT P1OUT |= FLASH_CS

#define MISO BIT5
#define MOSI BIT4
#define SCK BIT0

#define SCK_HIGH P3OUT |= SCK
#define SCK_LOW P3OUT &= ~SCK

#define SPI_READ (P3IN & MISO)

//void spi_setup(void (*spi_rx)(char));
void spi_setup();

/* SPI Transmit in Low Power Mode, Interrupt Unsafe */
unsigned char spi_tx_lpm_iu(unsigned char tx);

/* SPI Transmit in Active Power mode (interrupt safe) */
unsigned char spi_tx_am(unsigned char tx);

#endif /* SPI_H_ */
