/*
 *  twi.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#include <avr/io.h>
#include "twi.h"
#include <util/delay.h>



void i2c_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	while(!(TWCR&(1<<TWINT)));
}
void i2c_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	while((TWCR&(1<<TWSTO)));
}

void i2c_write(uint8_t bajt)
{
	TWDR = bajt;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t i2c_read(uint8_t ack)
{
	TWCR = (1<<TWINT) | (ack<<TWEA) | (1<<TWEN);
	while(!(TWCR&(1<<TWINT)));

	return TWDR;
}

void i2c_write_all_proc(uint8_t SLA,char *buf) //write- all procedure
{
	i2c_start();
	i2c_write(SLA);

	while(*buf!='\0')
	{
		i2c_write(*buf);
		buf++;

	}
	i2c_stop();
}

void i2c_read_all_proc(uint8_t SLA,uint8_t mem_adr, uint8_t len, uint8_t *buf)// read - all procedure
{
	i2c_start();
	i2c_write(SLA-1);
	i2c_write(mem_adr);
	i2c_start();
	i2c_write(SLA);

	while(len)
	{
		len--;
		/*
		 * if(len==0)
		 * 		i2c_read(NACK);
		 * else
		 * 		i2c_read(ACK);
		 */
		*buf = i2c_read(len ? ACK : NACK);
		buf++;

	}
	i2c_stop();
}

void i2cSetBitrate(uint8_t twbr,uint8_t prescaler) 
{
	switch (prescaler)
		{
			case 1:	//PRESCALER 1
				TWSR &= (~(1<<TWPS1)) | (~(1<<TWPS0));
			break;

			case 4:	//PRESCALER 4
				TWSR |= (1<<TWPS0);
			break;

			case 16:	//PRESCALER 16
				TWSR |= (1<<TWPS1);
			break;

			case 64:	//PRESCALER 64
				TWSR |= (1<<TWPS0) | (1<<TWPS1);
			break;

			default:
				break;
		}

	TWBR = twbr;


}

uint8_t change_dec_to_BCD(uint8_t number)
{
	return ( ((number / 10) << 4) | (number % 10) );
}

uint8_t change_BCD_to_number(uint8_t number_BCD)
{
	return  (((number_BCD >> 4) & 0x0F) * 10+((number_BCD) & (0x0F)));
}
