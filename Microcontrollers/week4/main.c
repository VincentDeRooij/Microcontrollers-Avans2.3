 #include <avr/io.h>
 #define F_CPU 8000000UL
 #include <util/delay.h>
 
 #define BIT(x) (1<<(x))

void opdrachtB1();
void opdrachtB2();
void opdrachtB3();
void opdrachtC();
void opdrachtC1();
void wait( int ms );

int main(void)
{
    /* Replace with your application code */
	opdrachtB3();
    while (1) 
    {
		
    }
}

void opdrachtB1() {
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output
	
	//init adc
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
	
	while (1)
	{
		PORTB = ADCL;			// Show MSB/LSB (bit 10:0) of ADC
		PORTA = ADCH;
		wait(100);				// every 100 ms (busy waiting)
	}
}

void opdrachtB2() {
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0x00;				// set PORTB for input
	
	//init adc
	
	while (1)
	{
		while (PINB > 0) {
			//init adc
			ADMUX = 0b01100011;			// AREF=VCC, result left adjusted, channel1 at pin PF3
			//laatste 4 bits van admux zijn om PF te selcteren
			ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
			//PORTB = ADCL;			// Show MSB/LSB (bit 10:0) of ADC
			PORTA = ADCH;
		}
		ADMUX = 0b00000000;
		ADCSRA = 0b00000000;
		PORTA = 0x00;
		wait(100);				// every 100 ms (busy waiting)
	}
}

void opdrachtB3() {
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output
	
	//init adc
	ADMUX = 0b01100011;			// AREF=VCC, result left adjusted, channel1 at pin PF3
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
	
	while (1)
	{
		PORTB = ADCL;			// Show MSB/LSB (bit 10:0) of ADC
		PORTA = ADCH;
		wait(100);				// every 100 ms (busy waiting)
	}
}

void opdrachtC() {
	
}

void opdrachtC1() {
	
}

void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 8 );		// library function (max 30 ms at 8MHz)
	}
}