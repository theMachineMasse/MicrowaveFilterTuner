/********************************************
*	Project: Microwave Filter Tuner
* File: MotorControlSubSystem
*	Date: January 27, 2020
*	Programmer(s): Matthew Rostad
* Sub-System: Motor Control Sub-System
*	Description: Contains the movement of the motors from Gcode commands
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "MotorControlSubSystem.h"
#include "Communications.h"


void moveX(int moveAmount){
	sendbyte(' ');
	sendbyte('X');
	printHex(moveAmount);
	sendbyte(' ');
}

void moveY(int moveAmount){
	sendbyte('Y');
	printHex(moveAmount);
}

void moveZ(int moveAmount){
	sendbyte('Z');
	printHex(moveAmount);
}


void moveXSlow(int moveAmount){
	sendbyte(' ');
	sendbyte('X');
	sendbyte('S');
	printHex(moveAmount);
	sendbyte(' ');
}

void moveYSlow(int moveAmount){
	sendbyte('Y');
	sendbyte('S');
	printHex(moveAmount);
}

void moveZSlow(int moveAmount){
	sendbyte('Z');
	sendbyte('S');
	printHex(moveAmount);
}

