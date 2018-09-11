/*
 *  spi.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#include <avr/io.h>
#include "can.h"
#include "spi.h"
#include <util/delay.h>




void InitSPI(void)
{
	//SETTING INPUTS AND OUTPUTS FOR MOSI,SCK,CS,MISO
	DDRB |= (1<<MOSI) | (1<<SCK) |(1<<CS)|(1<<CS_2);
	DDRB &= ~(1<<MISO);

	//SPI ACTIVATION, WORKING IN MASTER MODE
	SPCR |= (1<<SPE) | (1<<MSTR);
	// SPEED Fosc/2
	SPSR = (1<<SPI2X);
}

void SPI_sent(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

uint8_t SPI_receive(void)
{
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

uint8_t SPI_T_R(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
