/*
 *  can.c
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#include <avr/io.h>
#include "can.h"
#include "spi.h"
#include <util/delay.h>
#include "uart.h"

// CREATE PROTOCOL
CAN_Protocol Dane;


void CAN_module_write_register(uint8_t address, uint8_t data)
{
	PORTB &= ~(1<<CS);
	SPI_T_R(SPI_WRITE);
	SPI_T_R(address);
	SPI_T_R(data);
	PORTB |= (1<<CS);
}


uint8_t CAN_module_read_register(uint8_t address)
{
	uint8_t rec_data;
	PORTB &= ~(1<<CS);
	SPI_T_R(SPI_READ);
	SPI_T_R(address);
	rec_data = SPI_T_R(0xFF);
	PORTB |= (1<<CS);
	return rec_data;
}


void mcp2515_init(void)
    {


        //SETTING MODULE INTO CONFIGURATION MODE
        PORTB &= ~(1<<CS);
        SPI_T_R(SPI_RESET);
        _delay_ms(1);
        PORTB |= (1<<CS);
        _delay_ms(10);


        // CAN SPEED (125kHz)
        CAN_module_write_register(CNF1,(1<<BRP0)|(1<<BRP1)|(1<<BRP2));
        CAN_module_write_register(CNF2,(1<<BTLMODE)|(1<<PHSEG11));
        CAN_module_write_register(CNF3,(1<<PHSEG21));

        //SETTING INTERRUPT FOR RX BUFFER
        CAN_module_write_register(CANINTE,(1<<RX1IE)|(1<<RX0IE));

        // SETTING BUFFERS TO RECEIVING ALL POSSIBLE PROTOCOLS
        CAN_module_write_register(RXB0CTRL,(1<<RXM1)|(1<<RXM0));
        CAN_module_write_register(RXB1CTRL,(1<<RXM1)|(1<<RXM0));

        // MASKS SET TO 0 
        CAN_module_write_register(RXM0SIDH,0);
        CAN_module_write_register(RXM0SIDL,0);
        CAN_module_write_register(RXM0EID8,0);
        CAN_module_write_register(RXM0EID0,0);
        CAN_module_write_register(RXM1SIDH,0);
        CAN_module_write_register(RXM1SIDL,0);
        CAN_module_write_register(RXM1EID8,0);
        CAN_module_write_register(RXM1EID0,0);

        
        // RXnBF IN HIGH IMPEDANCE STATE
        CAN_module_write_register(BFPCTRL,0);
        // SETTIGN TXnRTS AS INPUTS
        CAN_module_write_register(TXRTSCTRL,0);
        //NORMAL MODE
        bit_modify(CANCTRL,0xE0,0);
    }


void bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
    PORTB &= ~(1<<CS);
    SPI_T_R(SPI_BIT_MODIFY);
    SPI_T_R(address);
    SPI_T_R(mask);
    SPI_T_R(data);
	PORTB |= (1<<CS);
}


void can_send_message(CAN_Protocol *p_message)
{
    uint8_t length = p_message->length;
    //SETTING BUFFER AS A HIGHEST PRIORITY 
    bit_modify(TXB0CTRL, (1<<TXP1)|(1<<TXP0), (1<<TXP1)|(1<<TXP0) );
    // SENDING PROTOCOL'S ID
    CAN_module_write_register(TXB0SIDH,(uint8_t)(p_message->id>>3));
    CAN_module_write_register(TXB0SIDL,(uint8_t)(p_message->id<<5));
    //  RTR
    if (p_message->rtr)
    {
    	CAN_module_write_register(TXB0DLC,(1<<RTR) | length);
    }
    else	//SENDING DATA
    {
        // LENGTH OF MESSAGE
    	CAN_module_write_register(TXB0DLC, length);
        // DATA
    	for (uint8_t i=0;i<length;i++)
    	{
        	CAN_module_write_register(TXB0D0 + i, p_message->data);
        }
    }
    // SENDING MESSAGE
    PORTB &= ~(1<<CS);
    SPI_T_R(SPI_RTS | 0x01);
    PORTB |= (1<<CS);
}


uint8_t mcp2515_read_rx_status(void)
{
    uint8_t rec_data;
    PORTB &= ~(1<<CS);
    SPI_T_R(SPI_RX_STATUS);
    rec_data = SPI_T_R(0xFF);

   
    SPI_T_R(0xFF);

    PORTB |= (1<<CS);
    return rec_data;
}


void can_get_message(CAN_Protocol *p_message)
{
    // READING STATUS REGISTER
    uint8_t status = mcp2515_read_rx_status();
    if (bit_is_set(status,6))		//CHECKING BUFFER
    {
    	PORTB &= ~(1<<CS);
    	SPI_T_R(SPI_READ_RX);
    }
    else if (bit_is_set(status,7))	//CHECKING SEC BUFFER
    {
    	PORTB &= ~(1<<CS);
    	SPI_T_R(SPI_READ_RX | 0x04);
    }
    else {
        return;	//NO MESSAGES
    }

    //JUMPING TO ADDRESS WHICH HAVE PROTOKOL'S ID
    p_message->id =  (uint16_t) SPI_T_R(0xFF) << 3;
    p_message->id |= (uint16_t) SPI_T_R(0xFF) >> 5;

    //JUMPING UNUSED REGISTERS
    SPI_T_R(0xFF);
    SPI_T_R(0xFF);

    //LENGTH
    uint8_t length = SPI_T_R(0xFF) & 0x0f;	
    p_message->length = length;

    //READING
    for (uint8_t i=0;i<length;i++)
    {
        p_message->data = SPI_T_R(0xFF);
    }

    PORTB |= (1<<CS);

    // CLEARING INTERRUPT'S FLAG
    if (bit_is_set(status,6))
    {
        bit_modify(CANINTF, (1<<RX0IF), 0);
    }
    else
    {
        bit_modify(CANINTF, (1<<RX1IF), 0);
    }
}


