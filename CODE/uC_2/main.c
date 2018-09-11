/*
 *  main.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#include <avr/io.h>
#include "can.h"
#include "twi.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"


//macros for pins setting 
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)

#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)

#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

#define PORT_INT A
#define PIN_INT PA1





int main(void)
{
	//setting CAN pin interrupt
	DDR(PORT_INT) &= ~(1<<PIN_INT); //setting pin as input 
	PORT(PORT_INT) |= (1<<PIN_INT); // pull up pin to VCC

	Dane.id = 0x12;
	Dane.rtr = 0;
	Dane.length =1;
	Dane.data ='O';

	InitSPI();	// SPI initialization
	mcp2515_init();	// CAN module initialization
	i2cSetBitrate(3,1);

	sei();	// setting Interrupt 


	while(1)
	{
		if(!(PIN(PORT_INT) & (1<< PIN_INT))) //Interrupt set flag when data appears 
		{
			can_get_message(&Dane);	//receiving data from CAN
			i2c_write_all_proc(ATMEGA_32_uC_3_SLAVE_ADRES,&Dane.data); //sending data using I2C
		}

	}
	return 0;
}






	







