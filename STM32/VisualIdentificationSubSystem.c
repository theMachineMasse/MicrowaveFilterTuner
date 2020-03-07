/********************************************
*	Project: Microwave Filter Tuner
* File: VisualIdentificationSubSystem
*	Date: February 20, 2020
*	Programmer(s): Matthew Rostad
* Sub-System: Visual Identification SubSystem
*	Description: Controls the lights and other visual identification systems
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "VisualIdentificationSubSystem.h"
#include "stm32f10x.h"
#include "Communications.h"


/*******************************************
*	Function: lightsInit
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Initializes the GPIO pins for lighting system
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void lightsInit(void){	
	GPIOC->CRH |= GPIO_CRH_MODE11 | GPIO_CRH_MODE12;
	GPIOC->CRH &= ~GPIO_CRH_CNF11 & ~GPIO_CRH_CNF12;
	
	GPIOC->BSRR |= GPIO_BSRR_BR11;
	GPIOC->BSRR |= GPIO_BSRR_BR12;
}

/*******************************************
*	Function: lightsToggle
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Turns the desired light on or off
*	Parameters: int pin, int state
*	Return value: N/A
*******************************************/
void lightsToggle(int pin, int state){
		
	sendbyte(' ');
	printHex(pin);
	sendbyte(' ');
	printHex(state);
	
	
	
	if(pin == 1){
		if(state == 0){
			GPIOC->BSRR |= GPIO_BSRR_BR11;
		}
		else{
			GPIOC->BSRR |= GPIO_BSRR_BS11;
		}
	}
	else if(pin == 2){
		if(state == 0){
			GPIOC->BSRR |= GPIO_BSRR_BR12;
		}
		else{
			GPIOC->BSRR |= GPIO_BSRR_BS12;
		}
	}
	
	
}
