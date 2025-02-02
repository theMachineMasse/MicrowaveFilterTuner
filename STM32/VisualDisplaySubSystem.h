/********************************************
*	Project: Microwave Filter Tuner
* File: VisualDisplaySubSystem.h
*	Date: January 27, 2020
*	Programmer(s): Braden Mass�
* Sub-System: Visual Display Sub-System
*	Description: Function declarations for all functions related to the Visual Display Sub-System (VDS)
* Version: 1.4
*	
********************************************/

/* INCLUDES */
#include "stm32f10x.h"


/* DEFINES */
#define LCD_CM_ENA 0x00210002  
#define LCD_CM_DIS 0x00230000 
#define LCD_DM_ENA 0x00200003 
#define LCD_DM_DIS 0x00220001 

#define LCD_CM_1L  0x80
#define LCD_CM_2L  0xC0
#define LCD_CM_3L  0x14
#define LCD_CM_4L  0x54

#define LCD_8B2L 0x38 // Enable 8 bit data, 2 display lines
#define LCD_DCB 0x0F //  Enable Display, Cursor, Blink
#define LCD_MCR 0x06 //  Set Move Cursor Right
#define LCD_CLR 0x01 //  Home and clear LCD
#define LCD_LN1 0x80 //  Set DDRAM to start of line 1
#define LCD_LN2 0xC0 //  Set DDRAM to start of line 2


/* FUNCTIONS */
void vdsInit(void);
void dataToLCD (uint8_t data);
void commandToLCD (uint8_t data);
void stringToLCD (char * message);
void lcdDisplayStatus (int statusCode);
void lcdDisplayError (int errorCode);
void delay(uint32_t count);
