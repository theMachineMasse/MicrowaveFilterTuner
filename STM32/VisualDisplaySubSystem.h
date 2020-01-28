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

void vds_Init(void);
void data_To_LCD (uint8_t data);
void command_To_LCD (uint8_t data);
void string_To_LCD (char * message);
void lcd_Display_Status (int statusCode);
void lcd_Display_Error (int errorCode);