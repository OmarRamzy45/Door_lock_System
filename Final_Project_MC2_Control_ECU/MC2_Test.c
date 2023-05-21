/******************************************************************************
 * CONTROL_MICRO_CONTROLLER MAIN FUNCTION TEST
 *
 * File Name: MC2_Test.c
 *
 * Description: Source file for the CONTROL_MC test
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#include "main2F.h"
#include <avr/io.h>
#include <util/delay.h>

/* global variables */
uint8 wrongPassTrials = 0;
uint8 g_seconds = 0;
uint8 recievedPass[PASS_SIZE];
uint8 storePass[PASS_SIZE];

int main(void) {

	uint8 i;

	SREG |= (1 << SREG_I); /*enable global interrupts */

	/*
	 * initialization of UART frame struct configuration to communicate with the other MC
	 * NO-Parity , one stop bit , 1-byte ( 8 bits ) word length and 9600 Baud rate
	 * */
	UART_ConfigType uart_config = { PARITY_DISABLED, OneStopBit, DataBits_8,9600 };
	UART_init(&uart_config);

	/*
	 * initialization of Timer driver configuration and choose to use Timer1 to be operated in compare mode
	 * use the prescaler as 1024 , initialize the value of prescaler2 to DONT Care as we don't use it
	 * set the initial value of the timer to 0 and compare value to 977 as we need the timer to raise flag
	 * each 1 sec , Timer freq = 8MHz/1024, one clock-cycle time = 128 MSecond
	 * so to force the timer to produce an interrupt every 1 second
	 we set the compare value to be 1/(128M) = 7813 */
	Timer_ConfigType timer1_config = { Timer1, 0, 7813, COMPARE_MODE, DONT_CARE,PRESCALER_1024 };
	Timer_init(&timer1_config);
	Timer_setCallBack(timer1HandlerCallBack, Timer1);

	/*initialize I2C driver */
	TWI_ConfigType twi_config = { CONTROL_ECU_ADD, PRESCALER__1, 0x02 };
	TWI_init(&twi_config);
	/* initialize motor and buzzer drivers */
	DcMotor_init();
	Buzzer_init();
	setPassFirstTime();

	uint8 recived_info = 0; /* variable to get the value from the other MC using UART */

	while (1) {
		if (UART_recieveByte() == READY_TO_SEND) {
			for (i = 0; i < PASS_SIZE; i++) {
				recievedPass[i] = UART_recieveByte();
				_delay_ms(100);
			}
			recived_info = UART_recieveByte();
		}
		/*
		 * switch case to check on the pressed key from the keypad if it is '+' which calls the open door mechanism
		 * or '-' which calls the change password mechanism
		 * */
		switch (recived_info) {
		case '+':
			if (checkPasswordMatch(storePass,
					recievedPass)== PASSWORDS_ARE_SAME) {
				UART_sendByte(DOOR_UNLOCK);
				openDoorOperation();
				/*
				 *  if the password is correct before
				 *  3 consecutive times you need to reset the value of wrong trials to 0
				 *  to start count again */
				wrongPassTrials = 0;
			}
			else {
				wrongPassTrials++;
				if (wrongPassTrials == MAX_WRONG_PASS_TRIALS) {
					UART_sendByte(ALERT);
					Buzzer_on();
					g_seconds = 0;
					while (g_seconds < ALERT_TIME) {}
					Buzzer_off();
					wrongPassTrials = 0;
				}
				else {
					UART_sendByte(WRONG_PASS);
				}
			}
			break;
		case '-':
			if (checkPasswordMatch(storePass,
					recievedPass) == PASSWORDS_ARE_SAME) {
				UART_sendByte(CHANGE_PASS);
				setPassFirstTime();
			}
			else {
				UART_sendByte(WRONG_PASS);
			}
			break;
		}

	}
}

/*
 * function to set the password for the first time
 * */
void setPassFirstTime(void) {
	uint8 i;

	uint8 checkPass[PASS_SIZE];
	uint8 checkFlag = 0;
	while (checkFlag == 0) {
		while (UART_recieveByte() != READY_TO_SEND) {
		}
		UART_sendByte(READY_TO_RECIEVE);
		for (i = 0; i < PASS_SIZE; i++) {
			recievedPass[i] = UART_recieveByte();
			_delay_ms(100);
		}
		while (UART_recieveByte() != READY_TO_SEND) {
		}
		UART_sendByte(READY_TO_RECIEVE);
		for (i = 0; i < PASS_SIZE; i++) {
			checkPass[i] = UART_recieveByte();
			_delay_ms(100);
		}

		if (checkPasswordMatch(recievedPass, checkPass) == PASSWORDS_ARE_SAME) {
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORDS_ARE_SAME);
			savePassInEEProm();
			checkFlag = 1;
		}
		else if (checkPasswordMatch(recievedPass,checkPass) == PASSWORDS_ARE_DIFF) {
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORDS_ARE_DIFF);
		}
	}
}

/*
 * function to compare between the two passwords and return true or false depending on
 * the reuslt of the compare
 * */
uint8 checkPasswordMatch(uint8 *pass1, uint8 *pass2) {
	updateStoredPassword();
	uint8 i;

	for (i = 0; i < PASS_SIZE; i++) {
		if (pass1[i] != pass2[i]) {
			return PASSWORDS_ARE_DIFF;
		}
	}
	return PASSWORDS_ARE_SAME;
}

/*
 * function to store the password inside the EEPROM
 * */
void savePassInEEProm(void) {
	uint8 i;
	for (i = 0; i < PASS_SIZE; i++) {
		EEPROM_writeByte(EEPROM_STORE_ADDREESS + i, recievedPass[i]);
		_delay_ms(100);
	}
}

/*
 * function that handles the opening door operation
 * */
void openDoorOperation(void) {
	g_seconds = 0;
	DcMotor_Rotate(CW);
	while (g_seconds < OPEN_TIME) {}
	g_seconds = 0;
	DcMotor_Rotate(STOP);
	while (g_seconds < HOLD_TIME) {}
	g_seconds = 0;
	DcMotor_Rotate(ACW);
	while (g_seconds < CLOSE_TIME) {}
	DcMotor_Rotate(STOP);
}

/*
 * function to update the password saved in EEPROM with the new given password
 * */
void updateStoredPassword(void) {
	uint8 i;
	for (i = 0; i < PASS_SIZE; i++) {
		EEPROM_readByte(EEPROM_STORE_ADDREESS + i, storePass + i);
	}
}

/*
 * the call back function of timer1
 * */
void timer1HandlerCallBack(void) {
	g_seconds++;
}
