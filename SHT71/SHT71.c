/*
 * SHT71.c
 *
 *  Created on: Oct 14, 2014
 *      Author: mdbalke
 */


#include <msp430.h>
#include "SHT71.h"

float read_temperature(){

	int raw_value;
	float temperature;

	const float D1 = -40.0;
	const float D2 = 0.018;

	raw_value = read_temperature_raw();

	temperature = (raw_value * D2) + D1;

	return temperature;
}

int read_temperature_raw(){

	int out;

	const int command = 0b00000011;

	send_SHT_command(command);

	DATA_IN;

	while(DATA_READ){
		_delay_cycles(10);
	}

	out = read_two_bytes_SHT();

	skipCrcSHT();

	return out;
}

void send_SHT_command(int command){

	int ack;

	DATA_OUT;
	SCL_OUT;

	//Send start sequence
	DATA_HIGH;
	SCL_HIGH;
	DATA_LOW;
	SCL_LOW;
	SCL_HIGH;
	DATA_HIGH;
	SCL_LOW;

	//Send command byte
	send_byte_SHT(command);

	//Look for ack
	SCL_HIGH;
	DATA_IN;
	ack = DATA_READ;

	if(ack != 0){
		//no ack
	}

	SCL_LOW;
	ack = DATA_READ;
	if(ack == 0){
		//ack error
	}
}

void send_byte_SHT(int command){
	int i;
	for(i = 7; i >= 0; i--){
		if(command & (1 << i)){
			DATA_HIGH;
		}else{
			DATA_LOW;
		}
		SCL_HIGH;
		SCL_LOW;
	}

}

int read_two_bytes_SHT(){

	int out;

	DATA_IN;

	out = read_byte_SHT();
	out = out << 8;

	//Send ack to sensor for first byte
	DATA_OUT;
	DATA_HIGH;
	DATA_LOW;
	SCL_HIGH;
	SCL_LOW;

	DATA_IN;
	out |= read_byte_SHT();

	return out;

}

unsigned char read_byte_SHT(){

	unsigned char out = 0;
	int i;
	for(i = 7; i >= 0; i--){
		SCL_HIGH;
		if(DATA_READ){
			out |= (1 << i);
		}
		SCL_LOW;
	}
	return out;
}

void skipCrcSHT(){

	DATA_OUT;
	SCL_OUT;

	DATA_HIGH;
	SCL_HIGH;
	SCL_LOW;

}


void SHT_data_in(){
	P2DIR &= ~BIT0;
	P2REN |= BIT0;
}
