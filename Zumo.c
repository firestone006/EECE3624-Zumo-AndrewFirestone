/*****************************************************************************
 *	    File: Lab10.c
 *  Lab Name: RS232/BT Communication Tester
 *    Author: Dr. Greg Nordstrom & Prof. John Hutton
 *   Created: 12/30/17
 *  Modified: 3/28/19, 1/17/21, 11/13/2023
 * Processor: ATmega128A (on the RegggadyAVR board)
 *
 * This program implements a communication server by reading a character
 * from a UART Terminal and reflecting that character to the BT UART
 * device.
 *
 * This program depends on the UARTLibrary used in previous labs, but
 * students are required to create and use their own UARTLibrary.c file,
 * but must use the UARTLibrary.h file that was given to them in Lab07.
 *
 * NOTE:  Since we skipped Lab08 this year (2023-Fall), I'm providing 
 * the USARTLibrary.a file from Lab07.
 *
 * In other words, their routines must conform to the prototypes they
 * have previously used.
 *
 * NB: The ReadyAVR-64 board uses UART1 as the default for programming,
 * freeing uart1 for external RS232 traffic (e.g. like adding the Bluetooth
 * transceivers in Lab09).
 *
 *   UART1 for communication to a Putty Terminal
 *   UART0 for communication across the paired BT channel.
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define END_STRING  '\0'
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>  // for sprintf functionality
#include "UARTLibrary.h"
#include "UART0Library.h"

//#define BAUDRATE    19200 // standard values from 1200 to 115200
//#define DATABITS        7 // 5-8
//#define STOPBITS        1 // 1,2
//#define PARITY          0 // 0=none, 1=odd, 2=even

#define BAUDRATE   9600 // standard values from 1200 to 115200
#define DATABITS      8 // 5-8
#define STOPBITS      1 // 1,2
#define PARITY        0 // 0=none, 1=odd, 2=even

volatile char joystickCommand = 0;

char text[40];

void send_string(const char *cp) {
	while(*cp != END_STRING) {
		uart_tx(*cp++);
	}
}

int main(void) {
	
	// Configure PD0-PD3 as inputs
	// Configure PD0-PD3 as inputs
	DDRD &= ~((1 << PD0) |
			(1 << PD1) |
			(1 << PD2) |
			(1 << PD3));

	// Enable pull-ups
	PORTD |= (1 << PD0) |
			(1 << PD1) |
			(1 << PD2) |
			(1 << PD3);

	// Configure PE4 as input
	DDRE &= ~(1 << PE4);

	// Enable PE4 pull-up
	PORTE |= (1 << PE4);

	// Configure INT0-INT3 for any logical change
	EICRA = 0b01010101;

	// Configure INT4 for any logical change
	EICRB = 0b01010101;

	// Clear pending interrupt flags
	EIFR = 0b00011111;

	// Enable INT0-INT4
	EIMSK |= 0b00011111;

	// Enable global interrupts
	sei();
	
	// For simplicity, use the same setup information for both ports.
	// Init UART1 for debug information to Putty
    uart_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	//uart_tx('a');
	
	// Init UART0 for BT/UART path
	uart0_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	//while(1) {
		//uart0_tx('a');
		//_delay_ms(1000);
	//}
	
	while(1)
	{
		if (joystickCommand != 0)
		{
			char command = joystickCommand;

			joystickCommand = 0;

			uart0_tx(command);
		}
	}
	return 0;
}

ISR(INT0_vect)
{
	if (!(PIND & (1 << PD0)))
		joystickCommand = 'R';
	else
		joystickCommand = 'S';
}
ISR(INT1_vect)
{
	if (!(PIND & (1 << PD1)))
		joystickCommand = 'B';
	else
		joystickCommand = 'S';
}

ISR(INT2_vect)
{
	if (!(PIND & (1 << PD2)))
		joystickCommand = 'L';
	else
		joystickCommand = 'S';
}

ISR(INT3_vect)
{
	if (!(PIND & (1 << PD3)))
		joystickCommand = 'F';
	else
		joystickCommand = 'S';
}
ISR(INT4_vect)
{
	if (!(PINE & (1 << PE4))){
		joystickCommand = 'X';
	}
}