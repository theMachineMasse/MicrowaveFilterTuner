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
#include "VisualIdentificationSubSystem.h"

int main(void) {
	clockInit();
	serial_open();
	motorInit();
	lightsInit();
	
	command();

	
	return 0;
}

