/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <util/delay.h>
#define F_CPU 8000000UL
#define BIT(x) (1<<(x))

typedef struct { //B5
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = { //B5
	{0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100},
	{0x00, 100},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0x00, 100},
	{0x81, 100}, {0x42, 100}, {0x24, 100}, {0x18, 100}, {0x0F, 200}, {0xF0, 200}, {0x0F, 200}, {0xF0, 200},
	{0x00, 0x00}
};


void wait(int);
void opdrachtB2();
void opdrachtB3();
void opdrachtB4();
void opdrachtB5();
void opdrachtB6();
void opdrachtB7();

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

	/* Insert application code here, after the board has been initialized. */
	
	DDRC = 0b00000000;					// PORTC.7 input all other bits output
	DDRD = 0b11111111;
	
	//opdrachtB6();
	opdrachtB7();
	
	while (1)
	{
		//opdrachtB2();
		//opdrachtB3();
		//opdrachtB4();
		//opdrachtB5();
	}

	return 1;
}

/******************************************************************/
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

void opdrachtB2() {
	PORTD ^= BIT(6);
	wait(500);
	PORTD ^= BIT(7);
	wait(500);
}

void opdrachtB3() {
	if (PINC == 0x80)
	{
		PORTD ^= BIT(7);
		//PORTD = 0b11111111;
		wait(500);
		PORTD = 0b00000000;
		wait(500);
	}
}

void opdrachtB4() {
	for (int i = 0; i <= 7 ;i++)
	{
		PORTD ^= BIT(i);
		wait(50);	
	}
}

void opdrachtB5() {
	// Set index to begin of pattern array
	int index = 0;
	// as long as delay has meaningful content
	while( pattern[index].delay != 0 ) {
		// Write data to PORTD
		PORTD = pattern[index].data;
		// wait
		wait(pattern[index].delay);
		// increment for next round
		index++;
	}
}

void opdrachtB6() {
	int delay = 1000;
	while (true) {
		if (PINC == 0b00000001) {
			if (delay == 1000) {
				delay = 250;
			} else {
				delay = 1000;
			}
		}
		PORTD ^= BIT(7);
		wait(delay);
		
	}
}

void opdrachtB7() {
	DDRA = 0b11111111;
	DDRB = 0b11111111;
	DDRC = 0b11111111;
	DDRD = 0b00000000;
	typedef enum {START, STATE1, STATE2, STATE3, END} ENUM_STATES;
	typedef enum {D5, D6, D7, NONE} ENUM_BUTTONS;
	//ENUM_STATES state = ENUM_STATES.START;
	ENUM_STATES currentstate = START;
	ENUM_BUTTONS btnpressed = NONE;

	while (true) {
		//PORTA = 0b11111111;
		//PORTB = 0b11111111;
		//PORTC = 0b11111111;
		//PORTD = 0b11111111;
		switch (PIND)
		{
		case 0b00100000:
			btnpressed = D5;
			break;
		case 0b01000000:
			btnpressed = D6;
			break;
		case 0b10000000:
			btnpressed = D7;
			break;
		default:
			btnpressed = NONE;
			break;
		}
		
		switch (currentstate)
		{
		case START:
			if (btnpressed == D5) {
				currentstate = STATE2;
			} else if (btnpressed == D6) {
				currentstate = STATE1;
			}
			PORTA = 0b00000001;
			break;
			
		case STATE1:
			if (btnpressed == D5) {
				currentstate = STATE2;
			} else if (btnpressed == D7) {
				currentstate = START;
			}
			PORTA = 0b00000010;
			break;
			
		case STATE2:
			if (btnpressed == D5) {
				currentstate = STATE3;
			} else if (btnpressed == D6) {
				currentstate = STATE1;
			} else if (btnpressed == D7) {
				currentstate = START;
			}
			PORTA = 0b00000100;
			break;
			
		case STATE3:
			if (btnpressed == D5) {
				currentstate = END;
			} else if (btnpressed == D6) {
				currentstate = END;
			} else if (btnpressed == D7) {
				currentstate = START;
			}
			PORTA = 0b00001000;
			break;
			
		case END:
			if (btnpressed == D7) {
				currentstate = START;
			}
			PORTA = 0b10000000;
			break;
		}
		wait(300);
	}
}