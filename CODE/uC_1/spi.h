/*
 *  spi.h
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#ifndef SPI_H_
#define SPI_H_

//FOR ATMEGA 32
#define MOSI PB5
#define MISO PB6
#define SCK	PB7
#define CS PB4
#define CS_2 PB3

void InitSPI(void);	// SPI INITIALIZATION
void SPI_sent(uint8_t data);	//SENDING SIGN
uint8_t SPI_receive(void);	//RECEIVING SIGN
uint8_t SPI_T_R(uint8_t data);	// SEND/RECEIVE SIGN SIMULTANEOUSLY

#endif /* SPI_H_ */
