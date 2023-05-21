/******************************************************************************
 * HMI_MICRO_CONTROLLER MAIN FUNCTION TEST
 *
 * File Name: main1F.h
 *
 * Description: Header file for the HMI_MC test
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#ifndef MAIN1F_H_
#define MAIN1F_H_

#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "Timer.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ENTER_BUTTON 13
#define PASS_SIZE 5
#define OPEN_TIME 15
#define CLOSE_TIME 15
#define HOLD_TIME 3
#define ALERT_TIME 60

#define ALERT 					(0x13)
#define READY_TO_SEND  			(0x10)
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

void getPass(uint8 *pass);
void setPassFirstTime(void);
void mainOptions(void);
void openDoorOperation(void);
void timer1HandlerCallBack(void);

#endif /* MAIN1F_H_ */
