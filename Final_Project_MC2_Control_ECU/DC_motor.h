 /******************************************************************************
 *
 * Module: DC_motor
 *
 * File Name: DC_motor.c
 *
 * Description: header file for the AVR DC_motor driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* DC_motors HW Ports and Pins Ids */
#define MOTOR_INPUT1_PORT_ID PORTC_ID
#define MOTOR_INPUT1_PIN_ID  PIN5_ID


#define MOTOR_INPUT2_PORT_ID PORTC_ID
#define MOTOR_INPUT2_PIN_ID  PIN6_ID



/*******************************************************************************
 *                               Typedef Definitions                            *
 *******************************************************************************/


typedef enum {
STOP,CW,ACW
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Function to setup the direction of the two pins of the motor and to be initialized at stop mode
 */
void DcMotor_init(void);

/*
 * Function is responsible for rotate the motor CW/ACW/STOP based on the state input
 * also send the required duty cycle for the PWM based on the required speed input
 * */

void DcMotor_Rotate(DcMotor_State state);



#endif /* DC_MOTOR_H_ */
