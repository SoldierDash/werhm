/*
 * sht71.h
 *
 *  Created on: Nov 17, 2014
 *      Author: mdbalke
 */

#ifndef SHT71_H_
#define SHT71_H_

#define SDA_PIN BIT4
#define SCL_PIN BIT5

#define DATA_HIGH P1OUT |= SDA_PIN
#define DATA_LOW P1OUT &= ~SDA_PIN
#define DATA_READ (P1IN & SDA_PIN)

#define DATA_IN SHT_data_in()
#define DATA_OUT P1DIR |= SDA_PIN

#define SCL_HIGH P1OUT |= SCL_PIN
#define SCL_LOW	P1OUT &= ~SCL_PIN
#define SCL_OUT P1DIR |= SCL_PIN


float read_temperature();
int read_temperature_raw();
void send_SHT_command(int command);
int read_two_bytes_SHT();
void skipCrcSHT();
void send_byte_SHT(int command);
unsigned char read_byte_SHT();

void SHT_data_in();

#endif /* SHT71_H_ */
