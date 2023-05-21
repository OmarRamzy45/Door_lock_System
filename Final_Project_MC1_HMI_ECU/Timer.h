 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Description: Header file for the Timer AVR driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

#define DONT_CARE 0xFF


typedef enum {
	Timer0,Timer1,Timer2
}Timer_ID;


typedef enum{
NO_CLOCK_STOP,PRESCALER_1,PRESCALER_8,PRESCALER_64,PRESCALER_256,PRESCALER_1024,EX_CS_FALLING,EX_CS_RISING
}Timer_Prescaler;


typedef enum {
NO_SOURCE_STOP,CLK_T2S_1,CLK_T2S_8,CLK_T2S_32,CLK_T2S_64,CLK_T2S_128,CLK_T2S_256,CLK_T2S_1024
}Timer2_Prescaler;


typedef enum{
NORMAL_MODE,COMPARE_MODE,FAST_PWM_MODE=0x03
}Timer_Mode;


typedef struct {
 Timer_ID timer_id ;
 uint16 initial_value;
 uint16 compare_value;
 Timer_Mode mode;
 Timer2_Prescaler prescaler2;
 Timer_Prescaler prescaler;
}Timer_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function to initialize the timer using dynamic configuration
 * */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Function to set the call back function of the chosen timerID
 * */
void Timer_setCallBack(void(*a_ptr)(void),Timer_ID ID);

/*
 * Function to disable a chosen timer by checking the TimerID
 * */
void Timer_deInit(Timer_ID ID);



#endif /* TIMER_H_ */
