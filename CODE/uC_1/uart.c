/*
 *  uart.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */



#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include <avr/interrupt.h>



volatile uint8_t uart_interrupt_delay;


ISR(USART_RXC_vect)	//INTERRUPT WHEN DATA IN BUF
{
	uint8_t new_head;
	char rec_data = UDR;
	new_head = (RX.UART_Head +1) & UART_RX_BUF_MASK;

	if(new_head == RX.UART_Tail)	//BUFFER OVERFLOW
	{
		PORTA ^= (1 << PA1);
		RX.UART_Head=0;
		RX.UART_Tail=0;
	}
	else	//SENDING DATA
	{
		{
			RX.UART_Head = new_head;
			RX.UART_Buf[RX.UART_Head] = rec_data;
			uart_interrupt_delay = 1;	
		}
	}
}


char UART_data_rx(void)	//CIRCULAR BUFFER
{
	if(RX.UART_Head==RX.UART_Tail)
		return '\0';
	/*
	if( RX.UART_Buf[RX.UART_Tail] == '' || RX.UART_Buf[RX.UART_Tail] == '_')
	{
		return '\0';
	}
	*/
	RX.UART_Tail = (RX.UART_Tail + 1) & UART_RX_BUF_MASK;
	return RX.UART_Buf[RX.UART_Tail];
}


void USART_Init(uint16_t baud)
{
	UBRRH = (uint8_t)(baud>>8);
	UBRRL = (uint8_t)baud;
	UCSRB |=(1<<RXCIE) | (1<<RXEN)|(1<<TXEN);	
	UCSRC |=(1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);	// 8N1
}


void UART_TRANSMIT(char data)
{
	//wait untill sending buffor will be empty
	while(!( UCSRA & (1<<UDRE)));  
	
	UDR = data;
}

void UART_TRANSMIT_STRING(char *string)
{

	while(*string)
	{
		UART_TRANSMIT(*string);
		string++;
	}
}

void UART_TRANMIST_INT(int data)
{
    int i =0;
	int j= 0;
	int copy = data;

	while(copy)
	{
		copy /=10;
		i++;
	}

	i++; //for NULL
	if(data < 0)
    {
       i++;// for '-';
    }

	char *int_to_string  = (char*)malloc(sizeof(char)*i);
	itoa(data, int_to_string,10);

	while(*int_to_string)
	{
		UART_TRANSMIT(*int_to_string);
		int_to_string++;
		j++;
	}
	int_to_string = int_to_string - j;
	free(int_to_string);
}


