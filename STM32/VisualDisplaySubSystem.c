/********************************************
*	Project: Microwave Filter Tuner
* File: VisualDisplaySubSystem.c
*	Date: January 27, 2020
*	Programmer(s): Braden Massé
* Sub-System: Visual Display Sub-System
*	Description: Contains all the functions to modify, update, and change the LCD and the LEDs
* Version: 1.3
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
	
	/* LED Initialization */
	GPIOB->CRH |= GPIO_CRH_MODE14 | GPIO_CRH_MODE15; //Green and Yellow LEDs
	GPIOB->CRH &= ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF15; //Green and Yellow LEDs
	
	GPIOC->CRH |= GPIO_CRH_MODE13; //Red LED
	GPIOC->CRH &= ~GPIO_CRH_CNF13; //Red LED
	
	GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
	GPIOB->BSRR |= GPIO_BSRR_BS15; // turn off YELLOW LED, need to double check
	GPIOC->BSRR |= GPIO_BSRR_BS13; // turn off RED LED, need to double check
	
	/* Start-up Commands */
	command_To_LCD(0x38); //NO MAGIC NUMBERS
  command_To_LCD(0x38); //NO MAGIC NUMBERS
  command_To_LCD(0x38); //NO MAGIC NUMBERS
	command_To_LCD(0x38); //NO MAGIC NUMBERS
	command_To_LCD(0x0F); //NO MAGIC NUMBERS
	command_To_LCD(0x01); //NO MAGIC NUMBERS
	command_To_LCD(0x06); //NO MAGIC NUMBERS
	
	/* Branding */
	string_To_LCD("University of Regina");
	command_To_LCD(0xC0);
	string_To_LCD("MFT v1.0");
	//add code to set to line 3
	string_To_LCD("Status: ");
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
	GPIOC->ODR &= 0xFF00; //NO MAGIC NUMBERS
	GPIOC->ODR |= data; 
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
	GPIOC->ODR &= 0xFF00; //NO MAGIC NUMBERS
	GPIOC->ODR |= data; 
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
			string_To_LCD("Status: Disconnected");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 1: 
			string_To_LCD("Status: Initializing");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 2: 
			string_To_LCD("Status: Ready");
			GPIOB->BSRR |= GPIO_BSRR_BR14; // turn on GREEN LED, need to double check
			break;
		case 3: 
			string_To_LCD("Status: Homing X,Y");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 4: 
			string_To_LCD("Status: Homing Phi");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 5: 
			string_To_LCD("Status: Moving X");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 6: 
			string_To_LCD("Status: Moving Y");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 7: 
			string_To_LCD("Status: Moving Z");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		case 8: 
			string_To_LCD("Status: Moving Phi");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
			break;
		default: 
			string_To_LCD("Status: Unknown");
			GPIOB->BSRR |= GPIO_BSRR_BS14; // turn off GREEN LED, need to double check
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
//add code to set cursor to line 4
	
	switch(errorCode) {
		case 11: 
			string_To_LCD("Error: 1-1 g_code");
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn on RED LED, need to double check
			break;
		case 12: 
			string_To_LCD("Error: 1-2 no screw");
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn on RED LED, need to double check
			break;
		case 13: 
			string_To_LCD("Error: 1-3 encoder");
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn on RED LED, need to double check
			break;
		case 14: 
			string_To_LCD("Error: 1-4 X home");
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn on RED LED, need to double check
			break;
		case 15: 
			string_To_LCD("Error: 1-5 Y home");
			GPIOC->BSRR |= GPIO_BSRR_BR13; // turn on RED LED, need to double check
			break;
		case 21: 
			string_To_LCD("Error: 2-1 no resp");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 22: 
			string_To_LCD("Error: 2-2 no screw");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 23: 
			string_To_LCD("Error: 2-3 F Y hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 24: 
			string_To_LCD("Error: 2-4 B Y hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 25: 
			string_To_LCD("Error: 2-5 L X hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 26: 
			string_To_LCD("Error: 2-6 R X hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 27: 
			string_To_LCD("Error: 2-7 U Z hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 28: 
			string_To_LCD("Error: 2-8 L Z hit");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 31: 
			string_To_LCD("Error: 3-1 g_code");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
		case 99: 
			string_To_LCD("                 ");
			GPIOB->BSRR |= GPIO_BSRR_BS15; // turn off YELLOW LED, need to double check
			GPIOC->BSRR |= GPIO_BSRR_BS13; // turn off RED LED, need to double check
			break;
		default: 
			string_To_LCD("Error: Unknown");
			GPIOB->BSRR |= GPIO_BSRR_BR15; // turn on YELLOW LED, need to double check
			break;
	}
//add code to set cursor back to line 3
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
