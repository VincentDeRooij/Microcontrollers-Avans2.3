/*
* Project name : Demo5_7a : spi - 7 segments display
* Author : Avans-TI, WvdE, JW
* Revision History : 20110228: - initial release; 20120307: - this version
* Description : This program sends data to a 4-digit display with spi
* Test configuration:
 MCU: ATmega128
 Dev.Board: BIGAVR6
 Oscillator: External Clock 08.0000 MHz
 Ext. Modules: Serial 7-seg display
 SW: AVR-GCC
* NOTES : Turn ON switch 15, PB1/PB2/PB3 to MISO/MOSI/SCK
*/
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select
// wait(): busy waiting for 'ms' millisecond - used library: util/delay.h
void wait(int ms)
{
for (int i=0; i<ms; i++)
_delay_ms(1);
}
void spi_masterInit(void)
{
DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
// Enable spi, MasterMode, Clock rate fck/64
// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
}
// Write a byte from master to slave
void spi_write( unsigned char data )
{
SPDR = data; // Load byte --> starts transmission
while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
}
// Write a byte from master to slave and read a byte from slave - not used here
char spi_writeRead( unsigned char data )
{
SPDR = data; // Load byte --> starts transmission
while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
data = SPDR; // New received data (eventually, MISO) in SPDR
return data; // Return received byte
}
// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
PORTB &= ~BIT(chipNumber);
}
// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
PORTB |= BIT(chipNumber);
}

void setDecodeModeOff() 
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(0x09); // Register 09: Decode Mode
	spi_write(0x00); // -> 1's = BCD mode for all digits
	spi_slaveDeSelect(0); // Deselect display chip
}

void setDecodeModeOn() 
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(0x09); // Register 09: Decode Mode
	spi_write(0xFF); // -> 1's = BCD mode for all digits
	spi_slaveDeSelect(0); // Deselect display chip
}

// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(0x09); // Register 09: Decode Mode
	spi_write(0xFF); // -> 1's = BCD mode for all digits
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select dispaly chip
	spi_write(0x0A); // Register 0A: Intensity
	spi_write(0x04); // -> Level 4 (in range [1..F])
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0B); // Register 0B: Scan-limit
	spi_write(0x03); // -> 1 = Display digits 0..3
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display on ('normal operation')
void displayOn()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display off ('shut down')
void displayOff()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x00); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}

void adjustDisplayBrightness(unsigned char value) // a value between 1 - 255
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0A); // Register 0B: Shutdown register
	spi_write(value); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}

void setValueForDigitDisplay(unsigned char digitDisplay, unsigned char value) 
{
	spi_slaveSelect(0); // Select display chip
	spi_write(digitDisplay); // Register 0B: Shutdown register
	spi_write(value); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}

void set_1_2_3_4_onDigitDisplays() 
{
	for (char i =4; i>0; i--)
	{
		setValueForDigitDisplay(i, 5-i);
		wait(500);
	}
}

void generateBrightnessLoop() 
{
	for (char i =0; i<=255; i++)
	{
		adjustDisplayBrightness(i);
		wait(500);
	}
}

void countUp() 
{
	char indexFirst = 0; // display 1
	char indexSecond = 0; // display 2
	char indexThirth = 0; // display 3
	char indexFourth = 0; // display 4
	
	for (char i = 0; i < 9999; i++ )
	{
		if (indexFirst == 9)
		{
			indexFirst = 0;
			indexSecond++;
			if (indexSecond == 9)
			{
				indexSecond = 0;
				indexThirth++;
				if (indexThirth == 9)
				{
					indexThirth = 0;
					indexFourth++;
				}
			}
			if (indexFourth == 9)
			{
				indexFirst = 0;
				indexSecond = 0;
				indexThirth = 0;
				indexFourth = 0;
			}
		}
		indexFirst++;
	
		setValueForDigitDisplay(1, indexFirst); // value for digit 1;
		setValueForDigitDisplay(2, indexSecond);
		setValueForDigitDisplay(3, indexThirth);
		setValueForDigitDisplay(4, indexFourth);
		
		wait(100);
	}
}

void writeCharacterMinusOnDisplay(unsigned char digitDisplay) 
{
	setValueForDigitDisplay(digitDisplay, 10);
}

void countDown() 
{
	char indexFirst = 0; // display 1
	char indexSecond = 0; // display 2
	char indexThirth = 0; // display 3
	char indexFourth = 0; // display 4
	
	for (int i = 9999; i > 0; i-- )
	{
		if (indexFirst == 0)
		{
			indexFirst = 9;
			indexSecond--;
			if (indexSecond == 0)
			{
				indexSecond = 9;
				indexThirth--;
				if (indexThirth == 0)
				{
					indexThirth = 9;
					indexFourth--;
				}
			}
			if (indexFourth == 0)
			{
				indexFirst = 9;
				indexSecond = 9;
				indexThirth = 9;
				indexFourth = 9;
			}
		}
		indexFirst--;
		
		setValueForDigitDisplay(1, indexFirst); // value for digit 1;
		setValueForDigitDisplay(2, indexSecond);
		setValueForDigitDisplay(3, indexThirth);
		setValueForDigitDisplay(4, indexFourth);
		
		wait(100);
	}
}

void clearDisplays()
{
	setDecodeModeOn();
	for (int i = 1; i <= 4; i++)
	{
		setValueForDigitDisplay(i, 15);
	}
}

void clearDisplay(unsigned char display)
{
	setValueForDigitDisplay(display, 15);
}

void showMinusDemo() 
{
		char minusToken = 10;
	
		char indexFirst = 9; // display 1
		char indexSecond = 9; // display 2
		char indexThirth = 9; // display 3
		char indexFourth = 9; // display 4
		
		indexFourth = minusToken;
		
		for (int i = 999; i > 0; i-- )
		{
			if (i <= 99)
			{
				clearDisplay(4);
				indexThirth = minusToken;	
			} 
			else if (i <= 9)
			{
				clearDisplay(3);
				indexSecond = minusToken;
			}
			
			if (indexFirst == 0)
			{
				indexFirst = 9;
				indexSecond--;
				if (indexSecond == 0)
				{
					indexSecond = 9;
					indexThirth--;
					if (indexThirth == 0)
					{
					}
				}
			}
			indexFirst--;
			
			setValueForDigitDisplay(1, indexFirst); // value for digit 1;
			setValueForDigitDisplay(2, indexSecond);
			setValueForDigitDisplay(3, indexThirth);
			setValueForDigitDisplay(4, indexFourth);
			
			wait(10);
		}
		
		countUp();
}

int main()
{
	DDRB=0x01; // Set PB0 pin as output for display select
	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip
	// clear display (all zero's)
	//adjustDisplayBrightness(200);
	//countDown();
	clearDisplays();
	showMinusDemo();
	//writeCharacterMinusOnDisplay(1);
	
	wait(1000);
	// write 4-digit data
	return (1);
}