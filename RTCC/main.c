/*** USCI master library ************************************************************

In this file the usage of the USCI I2C master library without DMA support is 
shown. This library uses pointers to specify what data is to be sent. 

When calling the TI_USCI_I2C_receive or TI_USCI_I2C_transmit routines 
the number of bytes, which are to be transmitted or received have to be passed as 
well as a pointer to a data field, that contains(or stores) the data.

This code checks if there is a slave with address 0x50 is connected to the I2C
bus and if the slave device is present, bytes are received and transmitted.

Uli Kretzschmar
MSP430 Systems
Freising
*******************************************************************************/
#include "msp430g2553.h"
#include "TI_USCI_I2C_master.h"

unsigned char timercounter;
unsigned char array[40] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb };
unsigned char store[40] = { 13, 13, 13, 13, 13, 13, 13, 0, 0, 0};

unsigned char RTC_addr = 0b1101111;
volatile int slave_present = 0;
unsigned char read_value = 96;

unsigned char time_keeping[2] = {0x00, 0xFF};


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  BCSCTL1 = CALBC1_8MHZ; 
  DCOCTL = CALDCO_8MHZ;

  _EINT();

  TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with USCI
  while ( TI_USCI_I2C_notready() );         // wait for bus to be free
  if ( TI_USCI_I2C_slave_present(RTC_addr) )    // slave address may differ from
  {                                         // initialization
	slave_present = 1;

    TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free
    TI_USCI_I2C_transmit(2,time_keeping);       // start transmitting
    while ( TI_USCI_I2C_notready() );

    TI_USCI_I2C_receiveinit(RTC_addr,0x12);   // init receiving with USCI
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free
    TI_USCI_I2C_receive(1,&read_value);
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free

  }
  
  LPM3;
 
}

