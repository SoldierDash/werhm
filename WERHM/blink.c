

#include "microcontroller.h"
//#include <msp430.h>

void flashLED();

int main(void) {

	disableWatchdog();

	setFrequency(0);

	setOutputPins(0x41); // Enable P1.0 and P1.6

	setInterruptPins(0x08); // Set P1.3 to interrupt pin, clears P1.3 flag

	sleepMode();
}


