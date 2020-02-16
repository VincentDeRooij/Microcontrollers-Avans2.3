#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define BIT(x) (1<<(x))

void opdrachtB2();
void opdrachtB3();
void opdrachtB4();
void opdrachtB5();
void displayDigit(int digit);
void init();
void lcd_writeChar( unsigned char dat );

const unsigned char PATTERN[] =
{
	// dP g f e d c b a
	0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001,
	0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,
	0b00000001,0b00000011,0b00000111,0b00001111,0b00011111,0b00111111,0b00011111,0b00001111,0b00000111,0b00000011,0b00000001,
	0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000,
	0b00000001,0b00000011,0b00000111,0b00001111,0b00011111,0b00111111,0b00011111,0b00001111,0b00000111,0b00000011,0b00000001,
	0b00100000,0b00010000,0b00001000,0b00000100,0b00000010,0b00000001,0b00000010,0b00000100,0b00001000,0b00010000,0b00100000
};

int b2Index;

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 8 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( INT0_vect )
{
    PORTA = (1<<b2Index);
	b2Index++;
	if (b2Index > 7) {
		b2Index = 0;
	}
}

ISR( INT1_vect )
{
    PORTD &= ~(1<<5);
	
}

int main( void )
{
	//board_init();
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input	

	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0b0000011;//0x03;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();				

	//opdrachtB2();
	opdrachtB3();
	//opdrachtB4();
	//opdrachtB5();

	while (1)
	{
								
	}

	return 1;
}

void opdrachtB2() {
	b2Index = 0;
	DDRA = 0b11111111;
}

void opdrachtB3() {
	DDRA = 0b00000000;
	DDRC = 0b11111111;
	int currentdigit = 0;
	
	displayDigit(0);
	
	while (1) {
		/*
		currentdigit++;
		displayDigit(currentdigit);
		wait(500);
		
		if (currentdigit > 15) {
			currentdigit = -1;
		}*/
		
		if (PINA & (1 << 0) && PINA & (1 << 1)) {
			//up and down pressed
			currentdigit = 0;
			displayDigit(currentdigit);
		} else if (PINA & (1 << 0)) {
			//down pressed
			currentdigit--;
			displayDigit(currentdigit);
		} else if (PINA & (1 << 1)) {
			//up pressed
			currentdigit++;
			displayDigit(currentdigit);
		}
		wait(250);
	}
}

void opdrachtB4() {
	DDRC = 0b11111111;                    // PORTD all output
	
	while (1==1)
	{
		// Set index to begin of pattern array
		// as long as delay has meaningful content
		for (int i = 0 ; i < sizeof(PATTERN) / sizeof(PATTERN[0]); i++ )
		{
			PORTC = PATTERN[i];
			wait(100);
		}
	}
}

void opdrachtB5() {
	init();
	
}

void displayDigit(int digit) {
	switch (digit) {
		case 0:
			PORTC = 0b00111111;
			break;
		case 1:
			PORTC = 0b00000110;
			break;
		case 2:
			PORTC = 0b01011011;
			break;
		case 3:
			PORTC = 0b01001111;
			break;
		case 4:
			PORTC = 0b01100110;
			break;
		case 5:
			PORTC = 0b01101101;
			break;
		case 6:
			PORTC = 0b01111101;
			break;
		case 7:
			PORTC = 0b00000111;
			break;
		case 8:
			PORTC = 0b01111111;
			break;
		case 9:
			PORTC = 0b01101111;
			break;
		case 10:
			PORTC = 0b01110111;
			break;
		case 11:
			PORTC = 0b01111100;
			break;
		case 12:
			PORTC = 0b00111001;
			break;
		case 13:
			PORTC = 0b01011110;
			break;
		case 14:
			PORTC = 0b01111001;
			break;
		case 15:
			PORTC = 0b01110001;
			break;
		default:
			PORTC = 0b11111001; //error
			break;
	}
}

