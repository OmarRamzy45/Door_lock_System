 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


typedef enum {
	PARITY_DISABLED,RESERVED,PARITY_EVEN,PARITY_ODD
}UART_Parity;

typedef enum {
	OneStopBit,TwoStopBit
}UART_StopBits;

typedef enum {
	DataBits_5,DataBits_6,DataBits_7,DataBits_8
}UART_DataBits;



typedef struct {

UART_Parity parity;
UART_StopBits stopBits ;
UART_DataBits dataBits ;
uint32 baudtRate  ;

}UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
