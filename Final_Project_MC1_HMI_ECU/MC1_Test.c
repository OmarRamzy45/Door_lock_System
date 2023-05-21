/******************************************************************************
 * HMI_MICRO_CONTROLLER MAIN FUNCTION TEST
 *
 * File Name: MC1_Test.c
 *
 * Description: Source file for the HMI_MC test
 *
 * Author: Abdelrahman Ali Mohamed
 *
 *******************************************************************************/

#include "main1F.h"
#include <util/delay.h>
#include <avr/io.h>

/* global variables */
uint8 input_pass[PASS_SIZE];
uint8 pass_cmp_result = 0;
uint8 numberOfWrongPass = 0;
uint8 g_seconds = 0;

int main(void) {
	uint8 key = 0, j = 0;

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
	 * each 1 sec , Timer freq = 1MHz/1024, one clock-cycle time = 1.024 mSecond
	 * so to force the timer to produce an interrupt every 1 second
	 we set the compare value to be 1/(1.024m) = 977 */
	Timer_ConfigType timer_config = { Timer1, 0, 977, COMPARE_MODE, DONT_CARE,PRESCALER_1024 };
	Timer_init(&timer_config);
	Timer_setCallBack(timer1HandlerCallBack, Timer1);/*set the timer function call back and choose Timer1*/

	LCD_init(); /*initialize the lcd */
	setPassFirstTime(); /* call the fucntion the set the password for the first time once the program starts*/
	mainOptions(); /* function to display options for the system on the lcd */
	uint8 recived_info = 0; /* variable to get the value from the other MC using UART */
	key = 0; /* variable to get the value of keypad button pressed */

	while (1) {

		key = KEYPAD_getPressedKey(); /* get the value of the pressed button from keypad */
		/*
		 * switch case to check on the pressed key from the keypad if it is '+' which calls the open door mechanism
		 * or '-' which calls the change password mechanism
		 * */
		switch (key) {
		case '+':
			LCD_clearScreen();
			LCD_displayString("Enter Pass");
			/*call the function of getting password from the user and then send it to the other MC by UART
			 * to check the password match then depending on the recieved value from the other MC by the UART
			 * if the two passwords match then call the open door function
			 * if the two passwords don't match then display a "wrong password" on lcd and diplay the main options again  */
			getPass(input_pass);
			UART_sendByte(READY_TO_SEND);
			for (j = 0; j < PASS_SIZE; j++) {
				UART_sendByte(input_pass[j]);
				_delay_ms(100);
			}
			UART_sendByte('+'); /* to inform the other MC of the chosen option */
			recived_info = UART_recieveByte(); /* get the value from the other MC after comparing the two passwords */
			if (recived_info == DOOR_UNLOCK) {
				openDoorOperation();
			}
			else if (recived_info == WRONG_PASS) {
				LCD_clearScreen();
				LCD_displayString("Pass is Wrong !");
				_delay_ms(500);
			}
			else if (recived_info == ALERT) {
				LCD_clearScreen();
				LCD_displayString("ALERT");
				g_seconds = 0;
				while (g_seconds < ALERT_TIME) {}
				g_seconds = 0;
			}
			mainOptions();
			break;
		case '-':
			LCD_clearScreen();
			LCD_displayString("Plz Enter old pass :");
			getPass(input_pass);
			UART_sendByte(READY_TO_SEND);
			for (j = 0; j < PASS_SIZE; j++) {
				UART_sendByte(input_pass[j]);
				_delay_ms(100);
			}
			UART_sendByte('-'); /* to inform the other MC of the chosen option */
			recived_info = UART_recieveByte(); /* get the value from the other MC after comparing the two passwords */
			if (recived_info == CHANGE_PASS) {
				setPassFirstTime(); /* set the new password */
				LCD_clearScreen();
			}
			else if (recived_info == WRONG_PASS) {
				LCD_clearScreen();
				LCD_displayString("Pass is Wrong !");
				_delay_ms(500);
			}
			mainOptions();
		}
	}

}

/*
 * function to display the main system options for the user to choose
 * */
void mainOptions(void) {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : Open Door ");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass ");
}

/*
 * function to set the password for the first time
 * */
void setPassFirstTime(void) {
	uint8 j = 0;
	while (pass_cmp_result == PASSWORDS_ARE_DIFF) {
		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass :");
		getPass(input_pass);
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECIEVE) {}
		for (j = 0; j < PASS_SIZE; j++) {
			UART_sendByte(input_pass[j]);
			_delay_ms(100);
		}

		LCD_clearScreen();
		LCD_displayString("RE-Enter pass :");
		getPass(input_pass);
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECIEVE) {}
		for (j = 0; j < PASS_SIZE; j++) {
			UART_sendByte(input_pass[j]);
			_delay_ms(100);
		}

		while (UART_recieveByte() != READY_TO_SEND) {}
		pass_cmp_result = UART_recieveByte();

		if (pass_cmp_result == PASSWORDS_ARE_DIFF) {
			LCD_clearScreen();
			LCD_displayString("passwords are diff ");
			_delay_ms(500);
		}
	}
	pass_cmp_result = PASSWORDS_ARE_DIFF; /* to use this function again when chose change password operation*/
}

/*
 * function to get the password entered by the user using keypad and store it in an array
 * */
void getPass(uint8 *pass) {
	uint8 key = 0, i = 0;
	LCD_moveCursor(1, 0);
	while (i != 5) {
		key = KEYPAD_getPressedKey();

		if ((key >= 0) && (key <= 9)) {
			*(pass + i) = key;
			LCD_displayCharacter('*');
			i++;
		}
		_delay_ms(500);
	}
	key = 0;
	while (KEYPAD_getPressedKey() != ENTER_BUTTON) {}

}

/*
 * function that handles the opening door operation
 * */
void openDoorOperation(void) {
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayString("unlocking door");
	while (g_seconds < OPEN_TIME) {}
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayString("door is open ");
	while (g_seconds < HOLD_TIME) {}
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayString("locking door ");
	while (g_seconds < CLOSE_TIME) {}
}

/*
 * the call back function of timer1
 * */
void timer1HandlerCallBack(void) {
	g_seconds++;
}

