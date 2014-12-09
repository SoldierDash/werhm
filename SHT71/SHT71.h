/*
 * SHT71.h
 *
 *  Created on: Oct 14, 2014
 *      Author: mdbalke
 */

#ifndef _SHT71_H_
#define _SHT71_H_

volatile float global_temperature;
volatile float global_humidity;

#define SDA_PIN BIT4
#define SCL_PIN BIT5

//Macros for data and clock lines
#define DATA_HIGH P1OUT |= SDA_PIN
#define DATA_LOW P1OUT &= ~SDA_PIN
#define DATA_READ (P1IN & SDA_PIN)

#define DATA_IN SHT_data_in()
#define DATA_OUT P1DIR |= SDA_PIN

#define SCL_HIGH P1OUT |= SCL_PIN
#define SCL_LOW	P1OUT &= ~SCL_PIN
#define SCL_OUT P1DIR |= SCL_PIN

float read_humidity();
float read_humidity_raw();
float read_temperature();
int read_temperature_raw();
void send_SHT_command(int command);
int read_two_bytes_SHT();
void skipCrcSHT();
void send_byte_SHT(int command);
unsigned char read_byte_SHT();

void SHT_data_in();
void blink_red();
void blink_green();

#endif /* _SHT71_H_ */
