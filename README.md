Author: Mateusz Michalski

Description:
A route registration system for UAV, based on AVR microcontrollers is a platform consisting of three ATmega microcontrollers, GPS module and an MicroSD card drive. One of the aims of the project was to learn as much communication interfaces as possible, that is why the project includes UART, I2C, SPI, CAN interfaces. All code has been written in C language. To communicate with MicroSD card drive "FatFs" library has been used. In addition, to make communication with CAN, MCP2515 module and MCP2551 transciver have been used. Working with this components reqiured to attached elements from "" library into the project.
Because project was made mostly for a educational purposes, additionaly, two PCB projects were designed and produced. One of them was a shrinked ATmega 32 starter kit and the second was the board to make communication with CAN between uC(board includes MCP2515 module and MCP2551 transciver).

The project was made as diploma thesis.
