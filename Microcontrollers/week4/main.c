 #include <avr/io.h>
 #define F_CPU 8000000UL
 #include <util/delay.h>

void opdrachtB();
void opdrachtB1();
void opdrachtB2();
void opdrachtB3();
void opdrachtC();
void opdrachtC1();
void wait( int ms );

int main(void)
{
    /* Replace with your application code */
	opdrachtB();
    while (1) 
    {
    }
}

void opdrachtB() {
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

void opdrachtB1() {
	
}

void opdrachtB2() {
	
}

void opdrachtB3() {
	
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