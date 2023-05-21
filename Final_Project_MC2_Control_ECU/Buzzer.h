 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.h
 *
 * Description: Header file for the AVR Buzzer driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_


/* Buzzer HW Ports and Pins Ids */
#define BUZZER_PORT_ID PORTC_ID
#define BUZZER__PIN_ID PIN3_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Function to initialize the buzzer pins and make it's first state is off*/
void Buzzer_init(void);

/* Function to set the buzzer on*/
void Buzzer_on(void);

/* Function to set the buzzer off*/
void Buzzer_off(void);


#endif /* BUZZER_H_ */
