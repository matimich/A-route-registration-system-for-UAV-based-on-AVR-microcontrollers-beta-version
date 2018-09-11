/*
 *  can.h
 *
 *  Author	: Mateusz Michalski
 *  uC	 	: Atmel AVR ATmega 32
 *  Compiler 	: avr-gcc
 */

#ifndef CAN_H_
#define CAN_H_

//REGISTER TO COMMUNICATE BETWEEN MODUL AND UC
#define SPI_WRITE 0x02
#define SPI_READ 0x03
#define SPI_RESET 0xC0
#define SPI_RTS 0x80
#define	SPI_RX_STATUS 0xB0
#define	SPI_READ_RX 0x90
#define	SPI_BIT_MODIFY	0x05

//REGISTER USED TO SET CAN SPEED
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A

//PRESCALER 
#define BRP2 2
#define BRP1 1
#define BRP0 0

//TIME SEGMENTS
#define BTLMODE	7
#define PHSEG11	4
#define PHSEG21	1

//INTERRUPTS
#define CANINTE		0x2B
#define RX1IE		1
#define RX0IE		0

//PROTOCOLS SETTINGS
#define RXB0CTRL	0x60
#define RXB1CTRL	0x70
#define RXM1		6
#define RXM0		5

//PINOUTS
#define BFPCTRL		0x0C
#define TXRTSCTRL	0x0D

//NORMAL MODE
#define CANCTRL		0x0F

//SENDING STANDARD PROTOCOL
#define TXB0SIDH	0x31
#define TXB0SIDL	0x32

//LENGTH OF MESSAGE, RTR
#define TXB0DLC		0x35
#define	RTR			6
#define TXB0D0		0x36


//PRIORITY OF MESSAGE
#define TXB0CTRL	0x30
#define TXP1		1
#define TXP0		0

//MASKS FOR IDENTIFICATORS
#define RXM0SIDH	0x20
#define RXM0SIDL	0x21
#define RXM0EID8	0x22
#define RXM0EID0	0x23
#define RXM1SIDH	0x24
#define RXM1SIDL	0x25
#define RXM1EID8	0x26
#define RXM1EID0	0x27


// RX INTERRUPT FLAGS
#define CANINTF		0x2C
#define RX1IF		1
#define RX0IF		0


typedef struct	//STRUCT FOR PROTOCOL 
{
    uint16_t  id;
    uint8_t   rtr;
    uint8_t   length;
    char   	  data;
}CAN_Protocol;

extern CAN_Protocol Dane;


//FUNCTIONS
void CAN_module_write_register(uint8_t address, uint8_t data); //SEND REGISTER
uint8_t CAN_module_read_register(uint8_t address);	//RECEIVE REGISTER
void bit_modify(uint8_t address, uint8_t mask, uint8_t data);	// BIT MODIFY
void mcp2515_init(void);	//MODULE INICIALIZATION
void can_send_message(CAN_Protocol *p_message);	//SEND MESSAGE
void can_get_message(CAN_Protocol *p_message);	//RECEIVE MESSAGE
uint8_t mcp2515_read_rx_status(void);	// READ RX STATUS



#endif /* CAN_H_ */
