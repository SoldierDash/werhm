


#include <msp430.h>
#include "microcontroller.h"
#include "interrupt.h"

#include "spi.h"

int main(void) {

	mcu_setup();
	spi_setup();

	while(1) {
		volatile int i = 5000;
		while(i)
			i--;
		//spi_tx_lpm_iu(0xAA);
	}
}

void
spi_rx(char rx) {
	if(rx == 0xAA) {
		led_flash();
	}
}
