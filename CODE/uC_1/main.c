/*
 *  main.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */


#include <avr/io.h>
#include "uart.h"
#include "can.h"
#include "spi.h"
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint8_t uart_interrupt_delay;

int main(void)
{
	Dane.id = 0x12;
	Dane.rtr = 0;
	Dane.length =1;
	Dane.data ='z';


	USART_Init(__UBRR);	// UART INITIALIZATION
	mcp2515_init();	//  CAN MODULE INITIALIZATION
	sei();	// INTERRUPT



	while(1)
	{

		if(uart_interrupt_delay ==1)	// DATA FROM GPS RECEIVED
		{
			Dane.data = UART_data_rx();
			can_send_message(&Dane);	//SENDING DATA 
			uart_interrupt_delay=0;		// FLAG = 0
		}

	}

	return 0;
}



