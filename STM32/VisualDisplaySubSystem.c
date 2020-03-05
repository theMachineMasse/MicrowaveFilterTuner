/********************************************
*	Project: Microwave Filter Tuner
* File: VisualDisplaySubSystem.c
*	Date: February 5, 2020
*	Programmer(s): Braden Massé
* Sub-System: Visual Display Sub-System
*	Description: Contains all the functions to modify, update, and change the LCD and the LEDs
* Version: 1.5
*	
********************************************/

/* INCLUDES */
#include "stm32f10x.h"
#include <string.h>
#include "VisualDisplaySubSystem.h"


/*******************************************
*	Function: vdsInit
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: To initialize the necessary pins, display branding on the LCD, 
*          and turn ON yellow LED
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void vdsInit(void) {
	/* LCD Initialization */
	GPIOC->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 ;
	GPIOC->CRL &= ~GPIO_CRL_CNF0 & ~GPIO_CRL_CNF1 & ~GPIO_CRL_CNF2 & ~GPIO_CRL_CNF3 & ~GPIO_CRL_CNF4 & ~GPIO_CRL_CNF5 & ~GPIO_CRL_CNF6 & ~GPIO_CRL_CNF7 ;
	
	GPIOB->CRL |= GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE5;
	GPIOB->CRL &= ~GPIO_CRL_CNF0 & ~GPIO_CRL_CNF1 & ~GPIO_CRL_CNF5;
	
	/* LED Initialization */
	GPIOB->CRH |= GPIO_CRH_MODE14 | GPIO_CRH_MODE15; //Green and Yellow LEDs
	GPIOB->CRH &= ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF15; //Green and Yellow LEDs
	
	GPIOC->CRH |= GPIO_CRH_MODE13; //Red LED
	GPIOC->CRH &= ~GPIO_CRH_CNF13; //Red LED
	
	GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
	GPIOB->BSRR |= GPIO_BSRR_BR14; // turn off YELLOW LED
	GPIOC->BSRR |= GPIO_BSRR_BR13; // turn off RED LED
	
	/* Start-up Commands */
	commandToLCD(LCD_8B2L);
  commandToLCD(LCD_8B2L);
  commandToLCD(LCD_8B2L);
	commandToLCD(LCD_8B2L);
	commandToLCD(LCD_DCB);
	commandToLCD(LCD_CLR);
	commandToLCD(LCD_MCR);
	
	/* Branding */
	commandToLCD(LCD_CM_1L); // move to line 1
	stringToLCD("University of Regina");
	commandToLCD(LCD_CM_2L); // move to line 2 
	stringToLCD("MFT v1.5"); 
	commandToLCD(0x80 | 0x14); // move to line 3
	lcdDisplayStatus(0); // initial status of disconnected
	commandToLCD(0xC); // turn off blinking cursor and underline
}


/*******************************************
*	Function: dataToLCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a character to the LCD
*	Parameters: uint8_t data
*	Return value: N/A
*******************************************/
void dataToLCD (uint8_t data) {
	GPIOB->BSRR = LCD_DM_ENA; //RS high, E high
	GPIOC->ODR &= 0xFF00;
	GPIOC->ODR |= data; 
	delay(8000);
	GPIOB->BSRR = LCD_DM_DIS; //RS high, E low
	delay(80000);
}


/*******************************************
*	Function: commandToLCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a command to the LCD
*	Parameters: uint8_t data
*	Return value: N/A
*******************************************/
void commandToLCD (uint8_t data) {
	GPIOB->BSRR = LCD_CM_ENA; //RS low, E high
	GPIOC->ODR &= 0xFF00;
	GPIOC->ODR |= data; 
	delay(8000);
	GPIOB->BSRR = LCD_CM_DIS; //RS low, E low
	delay(80000);
}


/*******************************************
*	Function: stringToLCD
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Send a string to the LCD
*	Parameters: char * message
*	Return value: N/A
*******************************************/
void stringToLCD (char * message) {
	int i=0;
	uint16_t messageLength = strlen(message);
	for (i=0; i<messageLength; ++i)
	{
		dataToLCD(*message);
		++message;
	}
}


/*******************************************
*	Function: lcdDisplayStatus
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Intepret the provided status code and update the current status on the LCD and LEDs
*	Parameters: int statusCode
*	Return value: N/A
*******************************************/
void lcdDisplayStatus (int statusCode) {

	commandToLCD(0x80 | 0x14);
	
	switch(statusCode) {
		case 0: 
			stringToLCD("Status: Disconnected");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 1: 
			stringToLCD("Status: Initializing");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 2: 
			stringToLCD("Status: Ready       ");
			GPIOB->BSRR |= GPIO_BSRR_BS15; // turn on GREEN LED
			break;
		case 3: 
			stringToLCD("Status: Homing X,Y,Z");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 4: 
			stringToLCD("Status: Homing Phi  ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 5: 
			stringToLCD("Status: Moving X    ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 6: 
			stringToLCD("Status: Moving Y    ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 7: 
			stringToLCD("Status: Moving Z    ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		case 8: 
			stringToLCD("Status: Moving Phi  ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
		default: 
			stringToLCD("Status: Unknown     ");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn off GREEN LED
			break;
	}	
}


/*******************************************
*	Function: lcdDisplayError
*	Programmer(s): Braden Massé
*	Date: January 27, 2020
*	Purpose: Intepret the provided error code and display the error on the LCD and turn on red LED
*	Parameters: int errorCode
*	Return value: N/A
*******************************************/
void lcdDisplayError (int errorCode) {

	commandToLCD(0x80 | 0x54);
	
	switch(errorCode) {
		case 11: 
			stringToLCD("Error: 1-1 g_code   ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 12: 
			stringToLCD("Error: 1-2 no screw ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 13: 
			stringToLCD("Error: 1-3 encoder  ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 14: 
			stringToLCD("Error: 1-4 X home   ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 15: 
			stringToLCD("Error: 1-5 Y home   ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 16: 
			stringToLCD("Error: 1-6 Z home   ");
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn on RED LED
			break;
		case 21: 
			stringToLCD("Error: 2-1 no resp  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 22: 
			stringToLCD("Error: 2-2 no screw ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 23: 
			stringToLCD("Error: 2-3 F Y hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 24: 
			stringToLCD("Error: 2-4 B Y hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 25: 
			stringToLCD("Error: 2-5 L X hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 26: 
			stringToLCD("Error: 2-6 R X hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 27: 
			stringToLCD("Error: 2-7 U Z hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 28: 
			stringToLCD("Error: 2-8 L Z hit  ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 31: 
			stringToLCD("Error: 3-1 g_code   ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
			break;
		case 99: 
			stringToLCD("                    ");
			GPIOB->BSRR |= GPIO_BSRR_BR14; // turn off YELLOW LED
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn off RED LED
			break;
		default: 
			stringToLCD("Error: Unknown      ");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn on YELLOW LED
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
