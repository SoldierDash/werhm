#include <msp430.h>

void blink();

unsigned char TXData;
unsigned char TXByteCtr;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = 0b1101111;
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	IE2 |= UCB0TXIE;                          // Enable TX interrupt

	TXData = 0x00;                            // Holds TX data

	while (1) {
		blink();
		TXByteCtr = 1;                          // Load TX byte counter
		//while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
		UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
		__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
												// Remain in LPM0 until all data
												// is TX'd
		TXData++;

	}
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	//blink();
	if (TXByteCtr)                            // Check TX byte counter
	{
		UCB0TXBUF = TXData;                     // Load TX buffer
		TXByteCtr--;                            // Decrement TX byte counter
	} else {
		UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
		IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
		__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
	}
}

void blink() {

	P1DIR |= BIT7;
	unsigned int i;
	for (i = 6; i != 0; i--) {
		P1OUT ^= BIT7;
		_delay_cycles(25000);
	}
	P1OUT &= ~BIT7;
}
