

#include "microcontroller.h"
//#include <msp430.h>

int main(void) {

	mcu_setup();

	while(1)
		led_flash();
}


