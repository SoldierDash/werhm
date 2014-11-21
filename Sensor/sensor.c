#include <msp430.h>				
#include "alarm.h"
#include "util.h"
#include "sht71.h"

void main() {

	init_clk();

	//Port1 config
	P1DIR = 0xFF;
	P1OUT = 0;

	seconds = 0;
	minutes = 0;
	hours = 0;

	while (1) {
		if (alarm == 1) {
			alarm = 0;
			blink_red();
		}

		//Put CPU to sleep in LPM mode 3, woken by interrupt
		_bis_SR_register(LPM3_bits + GIE);
	}
}

