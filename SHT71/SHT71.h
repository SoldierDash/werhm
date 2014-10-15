/*
 * SHT71.h
 *
 *  Created on: Oct 14, 2014
 *      Author: mdbalke
 */

#ifndef SHT71_H_
#define SHT71_H_


//Macros for data and clock lines
#define DATA_HIGH P2OUT |= BIT0
#define DATA_LOW P2OUT &= ~BIT0
#define DATA_READ (P2IN & BIT0)

#define DATA_IN SHT_data_in()
#define DATA_OUT P2DIR |= BIT0

#define SCL_HIGH P2OUT |= BIT1
#define SCL_LOW	P2OUT &= ~BIT1
#define SCL_OUT P2DIR |= BIT1


float read_temperature();
int read_temperature_raw();
void send_SHT_command(int command);
int read_two_bytes_SHT();
void skip_crc_SHT();
void send_byte_SHT(int command);
unsigned char read_byte_SHT();

void SHT_data_in();



#endif /* SHT71_H_ */
