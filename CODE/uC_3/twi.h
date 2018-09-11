/*
 *  twi.h
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */
#ifndef TWI_H_
#define TWI_H_

//ACKNOWLEDGE BITS
#define ACK 1
#define NACK 0

#define ATMEGA_32_uC_3_SLAVE_ADRES 0xD0 //SLAVE ADDRESS

//FUNCTIONS
void i2c_start(void);	//SENDING START BIT
void i2c_stop(void);	//SENDING STOP BIT
void i2c_write(uint8_t bajt);	//SENDING BIT
uint8_t i2c_read(uint8_t ack);	//RECEIVING BIT
void i2c_write_all_proc(uint8_t SLA,char *buf);	// PROCEDURE OF SENDING 
void i2c_read_all_proc(uint8_t SLA,uint8_t mem_adr, uint8_t len, uint8_t *buf);	//PROCEDURE OF RECEIVING
void i2cSetBitrate(uint8_t twbr,uint8_t prescaler);	//I2C SPEED


#endif /* TWI_H_ */
