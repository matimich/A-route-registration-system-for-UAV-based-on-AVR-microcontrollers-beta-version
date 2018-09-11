/*
 *  uart.h
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#ifndef UART_H_
#define UART_H_

#include <stdlib.h>

#define UART_RX_BUF_SIZE 256 //BUFFER SIZE 
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE-1) //MASK FOR BUFFER

//UART SPEED
#define UART_BAUD 4800	
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL*UART_BAUD)-1)



//RX BUF
struct
{
	 volatile char UART_Buf[UART_RX_BUF_SIZE];
	 volatile uint8_t UART_Head;
	 volatile uint8_t UART_Tail;
}RX;



void USART_Init(uint16_t baud);	// UART INITIALIZATION
char UART_data_rx(void);	//RECEIVE SIGN
void UART_TRANSMIT(char data);	// TRANSMIT SIGN
void UART_TRANSMIT_STRING(char *string);//TRANSMIT STRING
void UART_TRANMIST_INT(int data);	//TRANSMIT INT
//VAR
extern volatile uint8_t uart_interrupt_delay;


#endif /* UART_H_ */
