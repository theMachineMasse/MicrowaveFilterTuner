/********************************************
*	Project: Microwave Filter Tuner
* File: VisualDisplaySubSystem.h
*	Date: January 27, 2020
*	Programmer(s): Braden Massé
* Sub-System: Visual Display Sub-System
*	Description: Function declarations for all functions related tp the Visual Display Sub-System (VDS)
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "stm32f10x.h"


/* DEFINES */
#define LCD_CM_ENA 0x00210002  
#define LCD_CM_DIS 0x00230000 
#define LCD_DM_ENA 0x00200003 
#define LCD_DM_DIS 0x00220001 

#define LCD_8B2L 0x38 // Enable 8 bit data, 2 display lines
#define LCD_DCB 0x0F //  Enable Display, Cursor, Blink
#define LCD_MCR 0x06 //  Set Move Cursor Right
#define LCD_CLR 0x01 //  Home and clear LCD
#define LCD_LN1 0x80 //  Set DDRAM to start of line 1
#define LCD_LN2 0xC0 //  Set DDRAM to start of line 2


/* FUNCTIONS */
void vds_Init(void);
void data_To_LCD (uint8_t data);
void command_To_LCD (uint8_t data);
void string_To_LCD (char * message);
void lcd_Display_Status (int statusCode);
void lcd_Display_Error (int errorCode);
void delay(uint32_t count);
