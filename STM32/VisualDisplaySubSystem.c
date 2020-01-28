/********************************************
*	Project: Microwave Filter Tuner
* File: VisualDisplaySubSystem.c
*	Date: January 27, 2020
*	Programmer(s): Braden Massé
* Sub-System: Visual Display Sub-System
*	Description: Contains all the functions to modify, update, and change the LCD and the LEDs
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "stm32f10x.h"



/*******************************************
*	Function: vds_Init
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: To initialize the necessary pins, display branding on the LCD, 
*          and turn ON yellow LED
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void vds_Init(void) {
	
}


/*******************************************
*	Function: data_To_LCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a character to the LCD
*	Parameters: uint8_t data
*	Return value: N/A
*******************************************/
void data_To_LCD (uint8_t data) {
	
	
	
}


/*******************************************
*	Function: command_To_LCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a command to the LCD
*	Parameters: uint8_t data
*	Return value: N/A
*******************************************/
void command_To_LCD (uint8_t data) {
	
	
	
}


/*******************************************
*	Function: string_To_LCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a string to the LCD
*	Parameters: char * message
*	Return value: N/A
*******************************************/
void string_To_LCD (char * message) {
	
	
	
}


/*******************************************
*	Function: lcd_Display_Status
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Intepret the provided status code and update the current status on the LCD and LEDs
*	Parameters: int statusCode
*	Return value: N/A
*******************************************/
void lcd_Display_Status (int statusCode) {
	
	
}


/*******************************************
*	Function: lcd_Display_Error
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Intepret the provided error code and display the error on the LCD and turn on red LED
*	Parameters: int errorCode
*	Return value: N/A
*******************************************/
void lcd_Display_Error (int errorCode) {
	
	
}

