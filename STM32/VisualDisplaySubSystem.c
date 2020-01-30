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
#include <string.h>
#include "VisualDisplaySubSystem.h"


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
	/* LCD Initialization */
	GPIOC->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 ;
	GPIOC->CRL &= ~GPIO_CRL_CNF0 & ~GPIO_CRL_CNF1 & ~GPIO_CRL_CNF2 & ~GPIO_CRL_CNF3 & ~GPIO_CRL_CNF4 & ~GPIO_CRL_CNF5 & ~GPIO_CRL_CNF6 & ~GPIO_CRL_CNF7 ;
	
	GPIOB->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE5;
	GPIOB->CRL &= ~GPIO_CRL_CNF0 & ~GPIO_CRL_CNF1 & ~GPIO_CRL_CNF5;
	
	command_To_LCD(0x38);
  command_To_LCD(0x38);
  command_To_LCD(0x38);
	command_To_LCD(0x38);
	command_To_LCD(0x0F);
	command_To_LCD(0x01);
	command_To_LCD(0x06);
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
	GPIOB->BSRR = LCD_DM_ENA; //RS high, E high
	GPIOC->ODR &= 0xFF00; //GOOD: clears the low bits without affecting high bits
	GPIOC->ODR |= data; //GOOD: only affects lowest 8 bits of Port C
	delay(8000);
	GPIOB->BSRR = LCD_DM_DIS; //RS high, E low
	delay(80000);
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
	GPIOB->BSRR = LCD_CM_ENA; //RS low, E high
	GPIOC->ODR &= 0xFF00; //GOOD: clears the low bits without affecting high bits
	GPIOC->ODR |= data; //GOOD: only affects lowest 8 bits of Port C
	delay(8000);
	GPIOB->BSRR = LCD_CM_DIS; //RS low, E low
	delay(80000);
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
	int i=0;
	uint16_t messageLength = strlen(message);
	for (i=0; i<messageLength; ++i)
	{
		data_To_LCD(*message);
		++message;
	}
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
	
	switch(statusCode) {
		case 0: 
			break;
		
	}	
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
	
	switch(errorCode) {
		case 0: 
			break;
		
	}
}

/*******************************************
*	Function: delay
*	Programmer(s): Braden Massé
*	Date: January 29, 2020
*	Purpose: Produce a delay given a count
*	Parameters: uint32_t count
*	Return value: N/A
*******************************************/
void delay(uint32_t count)
{
    int i=0;
    for(i=0; i < count; i++)
    {
    }
}
