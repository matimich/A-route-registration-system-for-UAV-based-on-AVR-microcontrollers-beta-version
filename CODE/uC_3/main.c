/*
 *  main.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>

#include "spi.h"
#include "twi.h"
#include "ff.h"
#include "ffconf.h"
#include "integer.h"
#include "diskio.h"

#define Card_Check PA5


//for saving data on MicroSD card
BYTE drv=0;
FATFS FS;
FIL fp;

// initialize timer interrupt and variable
void timer1_init(void);
char volatile card_data; 
volatile uint8_t timer_interrupt_delay;
volatile uint8_t i2c_interrupt_delay;






ISR(TIMER1_COMPA_vect)
{
	static uint8_t add;
	add++;
	if(add ==100)	//setting flag to 10 sec 
	{
		timer_interrupt_delay =1;
		add=0;
	}

}


int main(void)
{

	DDRA &= ~(1 << PA5);


	/*
	if((PINA & (1<< Card_Check)))
	{
		PORTA |= (1 << PA0);
	}
	*/

	timer1_init();
	TwiSlaveInit(ATMEGA_32_uC_3_SLAVE_ADRES);
	InitSPI();

	disk_initialize(drv);
	f_mount(drv,&FS);
	f_open(&fp,"gps.txt", (	FA_OPEN_ALWAYS | FA_WRITE));
	sei();
	char var;
	while(1)
	{

		if(i2c_interrupt_delay == 1)	//INTERRUPT WHEN DATA APPEAR
		{
			var= card_data;
			f_lseek(&fp,fp.fsize);
			f_putc(var,&fp);
			i2c_interrupt_delay=0;

		}

		if(timer_interrupt_delay ==1)	//once in 10 sec, data are saved into the file
		{
			f_close(&fp);
			f_open(&fp,"gps.txt", (	FA_OPEN_ALWAYS | FA_WRITE));
			timer_interrupt_delay  =0;
		}

	}

	return 0;
}

void timer1_init(void)
{

    TCCR1B |= (1 << WGM12)|(1 << CS12);	// PRESKALER SET ON  256, CTC
	TIMSK  |= (1 << OCIE1A);	//INTERRUPT
	OCR1A = 6250;	// 100ms
}



ISR(TWI_vect)	//I2C INTERRUPT
{

	uint8_t TW = TWSR & 0xF8;	
	switch (TW)
	{
		case 0x60:	// SLAVE RECEIVED IT'S OWN ADDRESS
			TWCR |= (1<<TWEA) | (1<<TWINT); //PREPARING FOR RECEIVING ANOTHER BITS
			break;
		case 0x80:							//RECEIVING BITS
			card_data = TWDR;
			if(card_data == '\n') //END OF LINE
			{
				TWCR |= (1<<TWINT); //SENDING NACK, BUS IS TEMPORARY TURN OFF
				DDRB |= (1>>PB0);
				PORTB |= (1>>PB0);
			}
			else	//TAKING ANOTHER SIGNS
				TWCR |= (1<<TWEA) | (1<<TWINT);
			i2c_interrupt_delay = 1;
			break;
		case 0xA0:							//STOP BIT, STANDBY MODE
			TWCR |= (1<<TWINT) | (1<<TWEA);
			break;
		default:
			break;
	}
}


