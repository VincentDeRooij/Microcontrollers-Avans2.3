#include <asf.h>

void opdrachtB1();
void opdrachtB2();

int main (void)
{
	board_init();

	opdrachtB1();
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
	
}