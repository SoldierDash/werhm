
#include <msp430.h> 

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

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= (BIT0 + BIT6);
    P1OUT &= ~(BIT0 + BIT6);

    volatile float temperature;
    volatile float humidity;

	while(1){

		//humidity = read_humidity();

		temperature = read_temperature();

		//blink_red();

		blink_green();

		_delay_cycles(1000000);

	}
}

float read_humidity(){

	float linear_value;
	float raw_humidity;

	/*
	 * Conversion constants as defined in the datasheet
	 * 	for linearlizing sensor value
	 *
	 * 	H(linear) = C1 + C2*H(raw) + C3*H(raw)^2
	 */
	const float C1 = -2.0468;
	const float C2 = 0.0367;
	const float C3 = .0000015955;

	raw_humidity = read_humidity_raw();

	linear_value = C1 + C2 * raw_humidity + C3 * raw_humidity * raw_humidity;

	return linear_value;
}

float read_humidity_raw(){

	int out;

	const int command = 0b00000101;

	send_SHT_command(command);

	DATA_IN;

	while(DATA_READ){
		_delay_cycles(10);
	}

	out = read_two_bytes_SHT();

	skipCrcSHT();

	return out;
}

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

void blink_red(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT0;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT0;
}
void blink_green(){
	int i;
	for(i = 5; i >= 0; i--){
		P1OUT ^= BIT6;
		_delay_cycles(40000);
	}
	P1OUT &= ~BIT6;
}

void SHT_data_in(){
	P1DIR &= ~SDA_PIN;
	P1REN |= SDA_PIN;
}
