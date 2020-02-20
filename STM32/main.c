/********************************************
*	Project: Microwave Filter Tuner
* File: main.c
*	Date: January 27, 2020
*	Programmer(s): Braden Massé, Matthew Rostad
* Sub-System: N/A
*	Description: Main file
* Version: 1.0
*	
********************************************/
#include "Communications.h"
#include "MotorControlSubSystem.h"
#include "stm32f10x.h"

int main(void) {
	clockInit();
	serial_open();
	motorInit();
	
	command();
	//sendbyte(getbyte());

/*
	int freq = 4000;
	int delay = (1000000/freq)/2;
	
	while(1){
		GPIOA->BSRR |= GPIO_BSRR_BS7;
	delayUs(delay);
		GPIOA->BSRR |= GPIO_BSRR_BR7;
	delayUs(delay);
   

	}*/
	
	return 0;
}

