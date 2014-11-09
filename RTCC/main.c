
#include "msp430g2553.h"
#include "TI_USCI_I2C_master.h"

void blink_red();

void set_RTC_register(unsigned char addr, unsigned char value);
unsigned char read_RTC_register(unsigned char addr);

unsigned char RTC_addr = 0b1101111;
volatile int slave_present = 0;
unsigned int read_value = 96;
unsigned int reg_read = 0;

unsigned char time_keeping[2] = {0x00, 0x3A};

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;

  BCSCTL1 = CALBC1_8MHZ; 
  DCOCTL = CALDCO_8MHZ;

  _EINT();


  blink_red();

  TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with USCI
  while ( TI_USCI_I2C_notready() );         // wait for bus to be free

  if ( TI_USCI_I2C_slave_present(RTC_addr) )    // slave address may differ from
  {                                         // initialization
	slave_present = 1;

	/*
    TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free
    TI_USCI_I2C_transmit(2,time_keeping);       // start transmitting
    while ( TI_USCI_I2C_notready() );
	*/

	set_RTC_register(0x00, 0x3E);

	//reg_read = read_RTC_register(0x00);


    TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free
    TI_USCI_I2C_transmit(1,time_keeping);       // start transmitting
    while ( TI_USCI_I2C_notready() );

    TI_USCI_I2C_receiveinit(RTC_addr,0x12);   // init receiving with USCI
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free
    TI_USCI_I2C_receive(1,&read_value);
    while ( TI_USCI_I2C_notready() );         // wait for bus to be free

  }
  
  LPM3;
 
}

unsigned char read_RTC_register(unsigned char addr){

	unsigned char out = 0;

	unsigned char temp[1];
	temp[0] = addr;

	//Transmit control byte and requested register address
	TI_USCI_I2C_transmitinit(RTC_addr,0x12);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_transmit(1,temp);
	while ( TI_USCI_I2C_notready() );

	//Read in register value
	TI_USCI_I2C_receiveinit(RTC_addr,0x12);
	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_receive(1,&out);
	while ( TI_USCI_I2C_notready() );

	return out;
}



void set_RTC_register(unsigned char addr, unsigned char value){

	unsigned char out[2];
	out[0] = addr;
	out[1] = value;

	TI_USCI_I2C_transmitinit(RTC_addr,0x12);  // init transmitting with USCI
	while ( TI_USCI_I2C_notready() );         // wait for bus to be free

	if ( TI_USCI_I2C_slave_present(RTC_addr) ){
		TI_USCI_I2C_transmitinit(RTC_addr,0x12);
		while ( TI_USCI_I2C_notready() );
		TI_USCI_I2C_transmit(2, out);
		while ( TI_USCI_I2C_notready() );
	}

}

void blink_red(){

	unsigned int i;
	P1DIR |= BIT0;
	for(i = 6; i != 0; i--){
		P1OUT ^= BIT0;
		_delay_cycles(250000);
	}
	P1OUT &= ~BIT0;
}
