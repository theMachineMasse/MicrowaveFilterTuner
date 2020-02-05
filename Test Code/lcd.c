#include <stdint.h>
#include "stm32f10x.h"
#include "lcd.h"
#include "clocks.h"


// Commands for Hitachi 44780 compatible LCD controllers
#define LCD_8B2L 0x38 // ; Enable 8 bit data, 2 display lines
#define LCD_DCB 0x0F // ; Enable Display, Cursor, Blink
#define LCD_MCR 0x06 // ; Set Move Cursor Right
#define LCD_CLR 0x01 // ; Home and clear LCD
#define LCD_LN1 0x80 // ;Set DDRAM to start of line 1
#define LCD_LN2 0xC0 // ; Set DDRAM to start of line 2

// Control signal manipulation for LCDs on 352/384/387 board
// PB0:RS PB1:ENA PB5:R/W*

#define LCD_CM_ENA 0x00210002 //
#define LCD_CM_DIS 0x00230000 //
#define LCD_DM_ENA 0x00200003 //
#define LCD_DM_DIS 0x00220001 //


void lcdInit(void)
{
	commandToLCD(0x38);
	commandToLCD(0x38);
	commandToLCD(0x38);
	commandToLCD(0x38);
	commandToLCD(0x0f);
	commandToLCD(0x01);
	commandToLCD(0x06);
}


/*
* Name: commandToLCD
* Type: PUBLIC
* Parameters: a single byte of command information for the LCD controller
* Returns: nothing
* Description: This function generates control timing and data signals to send one command byte to the LCD
*/


void commandToLCD(uint8_t data)
{
GPIOB->BSRR = LCD_CM_ENA;		//RS low, E high
// GPIOC->ODR = data; 			//BAD: may affect upper bits on port C
GPIOC->ODR &= 0xFF00; 			//GOOD: clears the low bits without affecting high bits
GPIOC->ODR |= data; 			//GOOD: only affects lowest 8 bits of Port C
delay(8000);
GPIOB->BSRR = LCD_CM_DIS;		//RS low, E low
delay(80000);
}

void dataToLCD(uint8_t data)
{
GPIOB->BSRR = LCD_DM_ENA;		//RS low, E high
// GPIOC->ODR = data; 			//BAD: may affect upper bits on port C
GPIOC->ODR &= 0xFF00; 			//GOOD: clears the low bits without affecting high bits
GPIOC->ODR |= data; 			//GOOD: only affects lowest 8 bits of Port C
delay(8000);
GPIOB->BSRR = LCD_DM_DIS;		//RS low, E low
delay(80000);
}