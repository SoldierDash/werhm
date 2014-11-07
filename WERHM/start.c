

#include "microcontroller.h"
#include "spi.h"
//#include <msp430.h>

void spi_rx(char);

int main(void) {

	mcu_setup();
	spi_setup(&spi_rx);

	while(1);
	//	led_flash();
}


