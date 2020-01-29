/*
 * Week1Microcontrollers.c
 *
 * Created: 29-Jan-20 13:52:30
 * Author : Vincent de Rooij
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0b01111111;					// PORTD.7 input all other bits output
	PORTC = 0x10;						// ??
	
	while (1)
	{
		if (PINC & 0x80)
		{
			PORTD = 0x01;				// write 1 to all the bits of PortD
		}
		else
		{
			PORTD = 0x00;				// write 0 to all the bits of PortD
		}
	}
}

