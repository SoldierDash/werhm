


#include <msp430.h>
#include "microcontroller.h"
#include "interrupt.h"

#include "spi.h"

void spi_rx(char);

int main(void) {

	mcu_setup();
	spi_setup(spi_rx);

	while(1);
}

void
spi_rx(char rx) {
	if(rx == 0xAA) {
		P1OUT ^= 0x01;
	}
}
