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

int main(void) {
	clockInit();
	serial_open();
	
	command();
	//sendbyte(getbyte());
	
	return 0;
}

