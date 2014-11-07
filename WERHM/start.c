

#include "microcontroller.h"
#include "spi.h"
#include "interrupt.h"
//#include <msp430.h>

void spi_rx(char);

int main(void) {

	mcu_setup();
	spi_setup(spi_rx);

	while(1);
}

void
spi_rx(char rx) {
	if(rx) {
		P1OUT ^= BIT0;
	}
}
