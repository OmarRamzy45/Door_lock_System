/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: Source file for the Timer AVR driver
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/
#include "Timer.h"
#include <avr/io.h>
#include "gpio.h"
#include "common_macros.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                                global pointers                                  *
 *******************************************************************************/
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;

/*******************************************************************************
 *                      Functions ISR                                           *
 *******************************************************************************/

ISR( TIMER0_OVF_vect) {
	if (g_callBackPtr0 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer0 overflow mode */
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr0(); */
	}
}

ISR( TIMER0_COMP_vect) {
	if (g_callBackPtr0 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer0 compare mode*/
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr0(); */
	}
}

ISR( TIMER1_OVF_vect) {
	if (g_callBackPtr1 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer1 overflow mode*/
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr1(); */
	}
}

ISR( TIMER1_COMPA_vect) {
	if (g_callBackPtr1 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer1 compare mode */
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr1(); */
	}
}

ISR( TIMER2_OVF_vect) {
	if (g_callBackPtr2 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer2 overflow mode*/
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr2(); */
	}
}

ISR( TIMER2_COMP_vect) {
	if (g_callBackPtr2 != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected in Timer2 compare mode  */
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr2(); */
	}
}

/*******************************************************************************
 *                      Functions definitions                                   *
 *******************************************************************************/

/*
 * Function to initialize the timer using dynamic configuration
 * */
void Timer_init(const Timer_ConfigType *Config_Ptr) {

	/* This function first checks the value of prescaler2 and if it is Dont care it means
	 * that timer2 is not used then checks the TimerID which could be timer0 or timer 1
	 * then initialize the corresponding registers with the values in that configuration
	 * */
	if ((Config_Ptr->prescaler2) == DONT_CARE) {

		if ((Config_Ptr->timer_id) == Timer0) {

			/* first choose the prescaler then choose the initial value to start count from
			 * which are same steps in any mode in the timer0
			 * */
			TCCR0 = (Config_Ptr->prescaler);
			TCNT0 = Config_Ptr->initial_value;

			/* second checks the mode chosen between Normal / compare / fast PWM
			 * to initialize the other registers and the compare value in the timer0 and enable each mode
			 * */
			if (((Config_Ptr->mode) == NORMAL_MODE)) {
				TCCR0 |= (1 << FOC0);
				TIMSK = (1 << TOIE0);
			}

			else if ((Config_Ptr->mode) == COMPARE_MODE) {
				OCR0 = Config_Ptr->compare_value;
				TCCR0 |= (1 << FOC0) | (1 << WGM01) | (1 << COM01);
				TIMSK = (1 << OCIE0);
			}

			else if ((Config_Ptr->mode) == FAST_PWM_MODE) {
				OCR0 = Config_Ptr->compare_value;
				GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);/* configure pwm pin as output pin*/
				TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01);

			}
		}

		/* first choose the prescaler then choose the initial value to start count from
		 * which are same steps in any mode in the timer1
		 * */
		else if ((Config_Ptr->timer_id) == Timer1) {

			TCNT1 = Config_Ptr->initial_value;
			/*depends on the mode chosen the register TCCR1B is initialized with the selected prescaler */
			TCCR1B = ((Config_Ptr->mode) << WGM12) | ((Config_Ptr->prescaler));

			/* second checks the mode chosen between Normal / compare / fast PWM
			 * to initialize the other registers and the compare value in the timer1 and enable each mode
			 * */
			if ((Config_Ptr->mode) == NORMAL_MODE) {
				TCCR1A = (1 << FOC1A);
				TIMSK = (1 << TOIE1);
			}
			else if ((Config_Ptr->mode) == COMPARE_MODE) {
				OCR1A = Config_Ptr->compare_value;
				TCCR1A = (1 << FOC1A) | (1 << COM1A1);
				TIMSK = (1 << OCIE1A);
			}
			else if ((Config_Ptr->mode) == FAST_PWM_MODE) {
				OCR0 = Config_Ptr->compare_value;
				GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT); /* configure pwm pin as output pin*/
				TCCR1A = (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
			}

		}
	}
	/* Then it checks the value of prescaler and if it is Dont care it means
	 * that timer2 is used in this case and not timer0 or timer1
	 * and also checks on the timerID to make sure it's timer2 chosen
	 * then initialize the corresponding registers with the values in that configuration
	 * */
	else if ((Config_Ptr->prescaler) == DONT_CARE) {

		if ((Config_Ptr->timer_id) == Timer2) {

			/* first choose the prescaler2 then choose the initial value to start count from
			 * which are same steps in any mode in the timer2
			 * */
			TCCR2 = (Config_Ptr->prescaler2);
			TCNT2 = Config_Ptr->initial_value;

			/* second checks the mode chosen between Normal / compare / fast PWM
			 * to initialize the other registers and the compare value in the timer2 and enable each mode
			 * */
			if (((Config_Ptr->mode) == NORMAL_MODE)) {
				TCCR2 |= (1 << FOC2);
				TIMSK = (1 << TOIE2);
			}

			else if ((Config_Ptr->mode) == COMPARE_MODE) {
				OCR2 = Config_Ptr->compare_value;
				TCCR2 |= (1 << FOC2) | (1 << WGM21) | (1 << COM21);
				TIMSK = (1 << OCIE2);
			}

			else if ((Config_Ptr->mode) == FAST_PWM_MODE) {
				OCR2 = Config_Ptr->compare_value;
				GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT); /* configure pwm pin as output pin*/
				TCCR2 |= (1 << WGM21) | (1 << WGM20) | (1 << COM21);

			}
		}

	}

}

/*
 * Function to disable a chosen timer by checking the TimerID
 * */

void Timer_deInit(Timer_ID ID) {
	/*depends on each timer chosen, the corresponding registers are de initialized and disabled
	 *and also make the global pointer value as NULL again to not be used accidentally
	 * */
	if (ID == Timer0) {
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
		CLEAR_BIT(TIMSK, OCIE0);
		CLEAR_BIT(TIMSK, TOIE0);
		g_callBackPtr0 = NULL_PTR;
	} else if (ID == Timer1) {
		TCNT1 = 0;
		OCR1A = 0;
		TCCR1A = 0;
		TCCR1B = 0;
		g_callBackPtr1 = NULL_PTR;
	} else if (ID == Timer2) {
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
		CLEAR_BIT(TIMSK, OCIE2);
		CLEAR_BIT(TIMSK, TOIE2);
		g_callBackPtr2 = NULL_PTR;
	} else {
		return;
	}

}

/*
 * Function to set the call back function of the chosen timerID
 * */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID ID) {

	if (ID == Timer0) {
		g_callBackPtr0 = a_ptr;
	} else if (ID == Timer1) {
		g_callBackPtr1 = a_ptr;
	} else if (ID == Timer2) {
		g_callBackPtr2 = a_ptr;
	} else {
		return;
	}

}

