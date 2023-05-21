 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.c
 *
 * Description: Source file for the AVR Buzzer driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#include "Buzzer.h"
#include "gpio.h"
#include "std_types.h"

/* Function to initialize the buzzer pins and make it's first state is off*/
void Buzzer_init(void){

	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER__PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER__PIN_ID,LOGIC_LOW);
}

/* Function to set the buzzer on*/
void Buzzer_on(void){

	GPIO_writePin(BUZZER_PORT_ID,BUZZER__PIN_ID,LOGIC_HIGH);
}

/* Function to set the buzzer off*/
void Buzzer_off(void){

	GPIO_writePin(BUZZER_PORT_ID,BUZZER__PIN_ID,LOGIC_LOW);
}
