 /******************************************************************************
 *
 * Module: DC_motor
 *
 * File Name: DC_motor.c
 *
 * Description: Source file for the AVR DC_motor driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#include "DC_motor.h"
#include "common_macros.h"
#include "std_types.h"

/*
 * Function to setup the direction of the two pins of the motor and to be initialized at stop mode
 */
void DcMotor_init(void){

	/*
	 * Initialize the motors pins as output pins
	 */
	GPIO_setupPinDirection(MOTOR_INPUT1_PORT_ID,MOTOR_INPUT1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_INPUT2_PORT_ID,MOTOR_INPUT2_PIN_ID,PIN_OUTPUT);

	/*
	* Initialize the motor state to be at stop mode at the beginning
	*/
	GPIO_writePin(MOTOR_INPUT1_PORT_ID,MOTOR_INPUT1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_INPUT2_PORT_ID,MOTOR_INPUT2_PIN_ID,LOGIC_LOW);

}

/*
 * Function is responsible for rotate the motor CW/ACW/STOP based on the state input
 * also send the required duty cycle for the PWM based on the required speed input
 * */

void DcMotor_Rotate(DcMotor_State state){

	switch(state) {
	case STOP :
		GPIO_writePin(MOTOR_INPUT1_PORT_ID,MOTOR_INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_INPUT2_PORT_ID,MOTOR_INPUT2_PIN_ID,LOGIC_LOW);
		break;
	case CW :
		GPIO_writePin(MOTOR_INPUT1_PORT_ID,MOTOR_INPUT1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_INPUT2_PORT_ID,MOTOR_INPUT2_PIN_ID,LOGIC_LOW);
		break;
	case ACW :
		GPIO_writePin(MOTOR_INPUT1_PORT_ID,MOTOR_INPUT1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_INPUT2_PORT_ID,MOTOR_INPUT2_PIN_ID,LOGIC_HIGH);
		break;
	}




}
