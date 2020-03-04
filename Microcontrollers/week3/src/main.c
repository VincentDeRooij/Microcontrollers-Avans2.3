#include <asf.h>
#include "lcd.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void opdrachtB1();
void opdrachtB2();
void timer2Init( void );

#define BIT(x) (1 << (x))
#define TimerPreset 1

int tenthValue;

ISR( TIMER3_OVF_vect )
{
	TCNT3 = TimerPreset; // Preset value
	tenthValue++; // Increment counter
}

ISR( INT6_vect )
{
	//lcd_clear_screen();
	//lcd_write_command(0x02);
	char c[20];
	sprintf(c, "%d", TCNT3);
	//lcd_write_string(c);
	lcd_overwrite_line(c, 0);
	//lcd_write_command(0x02);
}

int main (void)
{
	board_init();

	opdrachtB2();
}

void opdrachtB1() {
	DDRB |= (1 << 0); // Set LED as output

	TCCR1B |= (1 << CS10); // Set up timer
	
	for (;;)
	{
		// Check timer value in if statement, true when count matches 1/20 of a second
		if (TCNT1 >= 49999)
		{
			PORTB ^= (1 << 0); // Toggle the LED

			TCNT1 = 0; // Reset timer value
		}
	}
}

void opdrachtB2() {
	init_4bits_mode();
	lcd_clear_screen();
	
	DDRE &= ~(1<<6); // set PORTD.7 for input
	DDRA = 0xFF; // set PORTA for output (shows countregister)
	DDRB = 0xFF; // set PORTB for output (shows tenthvalue)
	
	EICRB = 0xB0;
	EIMSK = 0b01000000;
	
	//DDRE = 0x00;
	timer2Init();
	while (1)
	{
		PORTA = TCNT3; // show value counter 2
		PORTB = tenthValue; // show value tenth counter
		_delay_ms(10); // every 10 ms
	}
}

void timer2Init( void )
{
	TCNT3 = TimerPreset; // Preset value of counter 2
	TIMSK |= 1<<6; // T2 overflow interrupt enable
	TCCR3B = 0x07; // Initialize T3: ext.counting, rising edge, run
	sei(); // turn_on intr all
}