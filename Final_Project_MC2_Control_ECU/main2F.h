/******************************************************************************
 * CONTROL_MICRO_CONTROLLER MAIN FUNCTION TEST
 *
 * File Name: main2F.h
 *
 * Description: Header file for the CONTROL_MC test
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#ifndef MAIN2F_H_
#define MAIN2F_H_

#include "uart.h"
#include "Timer.h"
#include "std_types.h"
#include "twi.h"
#include "external_eeprom.h"
#include "DC_motor.h"
#include "Buzzer.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define CONTROL_ECU_ADD			(0x1)
#define EEPROM_STORE_ADDREESS	(0x00)

#define PASS_SIZE 5
#define OPEN_TIME 15
#define CLOSE_TIME 15
#define HOLD_TIME 3
#define MAX_WRONG_PASS_TRIALS 3
#define ALERT_TIME 60
#define MOTOR_PERIOD 15
#define HOLD_MOTOR_PERIOD 3

#define ALERT 					(0x13)
#define READY_TO_SEND 			(0x10)
#define READY_TO_RECIEVE 		(0x11)
#define PASSWORDS_ARE_SAME 		(1u)
#define PASSWORDS_ARE_DIFF 		(0u)
#define PASS_IS_CORRECT 		(0x12)
#define DOOR_UNLOCK				(0x25)
#define WRONG_PASS				(0x30)
#define CHANGE_PASS				(0X31)


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void setPassFirstTime(void);
uint8 checkPasswordMatch(uint8 *pass1,uint8 *pass2);
void savePassInEEProm(void);
void timer1HandlerCallBack(void);
void openDoorOperation(void);
void updateStoredPassword(void);


#endif /* MAIN2F_H_ */
