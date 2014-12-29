/*
 * CC.h
 *
 *  Created on: Dec 3, 2014
 *      Author: mdbalke
 */

#ifndef CC_1101_H_
#define CC_1101_H_

#define CC1101_MAX_FIFO 64

//Definition for SPI lines on port 1
#define CS BIT1
#define GDO0 BIT2
#define GDO2 BIT4

#define MAX_RXFIFO 64
#define MAX_TXFIFO 64

// CC1101 SPI Header: (R/!W) + (Burst) + (A5-A0)
#define CC_HEADER_RW	BIT7 // Read/!Write
#define CC_HEADER_BURST	BIT6 // Burst/!Single

#define CC_STATUS_FIFO(x)	(x & 0b00001111)
#define CC_STATUS_STATE(x)	(x & 0b01110000)

/*
 * Single TX FIFO	0x3F	0b00 111111
 * Burst TX FIFO	0x7F	0b01 111111
 * Single RX FIFO	0xBF	0b10 111111
 * Burst RX FIFO	0xFF	0b11 111111
 */

// PATABLE (0 dBm output power)
//char paTable[] = {0x51};
//char paTableLen = 1;


// Configuration Registers
#define CC_IOCFG2       0x00        // GDO2 output pin configuration
#define CC_IOCFG1       0x01        // GDO1 output pin configuration
#define CC_IOCFG0       0x02        // GDO0 output pin configuration
#define CC_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CC_SYNC1        0x04        // Sync word, high byte
#define CC_SYNC0        0x05        // Sync word, low byte
#define CC_PKTLEN       0x06        // Packet length
#define CC_PKTCTRL1     0x07        // Packet automation control
#define CC_PKTCTRL0     0x08        // Packet automation control
#define CC_ADDR         0x09        // Device address
#define CC_CHANNR       0x0A        // Channel number
#define CC_FSCTRL1      0x0B        // Frequency synthesizer control
#define CC_FSCTRL0      0x0C        // Frequency synthesizer control
#define CC_FREQ2        0x0D        // Frequency control word, high byte
#define CC_FREQ1        0x0E        // Frequency control word, middle byte
#define CC_FREQ0        0x0F        // Frequency control word, low byte
#define CC_MDMCFG4      0x10        // Modem configuration
#define CC_MDMCFG3      0x11        // Modem configuration
#define CC_MDMCFG2      0x12        // Modem configuration
#define CC_MDMCFG1      0x13        // Modem configuration
#define CC_MDMCFG0      0x14        // Modem configuration
#define CC_DEVIATN      0x15        // Modem deviation setting
#define CC_MCSM2        0x16        // Main Radio Cntrl State Machine config
#define CC_MCSM1        0x17        // Main Radio Cntrl State Machine config
#define CC_MCSM0        0x18        // Main Radio Cntrl State Machine config
#define CC_FOCCFG       0x19        // Frequency Offset Compensation config
#define CC_BSCFG        0x1A        // Bit Synchronization configuration
#define CC_AGCCTRL2     0x1B        // AGC control
#define CC_AGCCTRL1     0x1C        // AGC control
#define CC_AGCCTRL0     0x1D        // AGC control
#define CC_WOREVT1      0x1E        // High byte Event 0 timeout
#define CC_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CC_WORCTRL      0x20        // Wake On Radio control
#define CC_FREND1       0x21        // Front end RX configuration
#define CC_FREND0       0x22        // Front end TX configuration
#define CC_FSCAL3       0x23        // Frequency synthesizer calibration
#define CC_FSCAL2       0x24        // Frequency synthesizer calibration
#define CC_FSCAL1       0x25        // Frequency synthesizer calibration
#define CC_FSCAL0       0x26        // Frequency synthesizer calibration
#define CC_RCCTRL1      0x27        // RC oscillator configuration
#define CC_RCCTRL0      0x28        // RC oscillator configuration
#define CC_FSTEST       0x29        // Frequency synthesizer cal control
#define CC_PTEST        0x2A        // Production test
#define CC_AGCTEST      0x2B        // AGC test
#define CC_TEST2        0x2C        // Various test settings
#define CC_TEST1        0x2D        // Various test settings
#define CC_TEST0        0x2E        // Various test settings

// Status registers - read only
#define CC_PARTNUM      0x30        // Part number
#define CC_VERSION      0x31        // Current version number
#define CC_FREQEST      0x32        // Frequency offset estimate
#define CC_LQI          0x33        // Demodulator estimate for link quality
#define CC_RSSI         0x34        // Received signal strength indication
#define CC_MARCSTATE    0x35        // Control state machine state
#define CC_WORTIME1     0x36        // High byte of WOR timer
#define CC_WORTIME0     0x37        // Low byte of WOR timer
#define CC_PKTSTATUS    0x38        // Current GDOx status and packet status
#define CC_VCO_VC_DAC   0x39        // Current setting from PLL cal module
#define CC_TXBYTES      0x3A        // Underflow and # of bytes in TXFIFO
#define CC_RXBYTES      0x3B        // Overflow and # of bytes in RXFIFO
#define CC_NUM_RXBYTES  0x7F        // Mask "# of bytes" field in _RXBYTES

// Strobe commands
#define CC_SRES         0x30        // Reset chip.
#define CC_SFSTXON      0x31        // Enable/calibrate freq synthesizer
#define CC_SXOFF        0x32        // Turn off crystal oscillator.
#define CC_SCAL         0x33        // Calibrate freq synthesizer & disable
#define CC_SRX          0x34        // Enable RX.
#define CC_STX          0x35        // Enable TX.
#define CC_SIDLE        0x36        // Exit RX / TX
#define CC_SAFC         0x37        // AFC adjustment of freq synthesizer
#define CC_SWOR         0x38        // Start automatic RX polling sequence
#define CC_SPWD         0x39        // Enter pwr down mode when CSn goes high
#define CC_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CC_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CC_SWORRST      0x3C        // Reset real time clock.
#define CC_SNOP         0x3D        // No operation.




unsigned char CC1101_reg_write(unsigned char address, unsigned char data);
unsigned char CC1101_burst_reg_write(unsigned char starting_address, unsigned char *data, int num_bytes);
unsigned char CC1101_reg_read(unsigned char address);
unsigned char CC1101_strobe(unsigned char strobe);
unsigned char CC1101_read_status_register(unsigned char address);

void cc1101_config(unsigned char device_address, unsigned char channel_number);
void cc1101_send_packet(unsigned char *data, int num_bytes);
unsigned char cc1101_rcv_packet(unsigned char *data, int *num_bytes);

unsigned char CC1101_sleep_wake_on_radio();


unsigned char CC1101_burst_reg_read(unsigned char starting_address, unsigned char *data, int num_bytes);

#endif /* CC_1101_H_ */
