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
#include "stm32f10x.h"

//Globals
int xPosG = 0;
int yPosG = 0;
int zPosG = 0;
const int stepsPerMM = 400;
const int rampSize = 1900;
const int maxSpeed = 4000;
const int minSpeed = 200;
const int slowRampSize = 190;
const int slowMaxSpeed = 400;
const int slowMinSpeed = 40;


/*******************************************
*	Function: motorInit
*	Date: February 20, 2020
*	Purpose: Initializes necessary motor GPIO pins
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void motorInit(void){	
	
	
	//motor GPIO init
	GPIOA->CRL |= GPIO_CRL_MODE7 ;
	GPIOA->CRL &= ~GPIO_CRL_CNF7 ;
	GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15;
	GPIOA->CRH &= ~GPIO_CRH_CNF8 & ~GPIO_CRH_CNF9 & ~GPIO_CRH_CNF10 & ~GPIO_CRH_CNF11 & ~GPIO_CRH_CNF12 & ~GPIO_CRH_CNF13 & ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF15;
	
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
	GPIOB->CRL &= ~GPIO_CRL_MODE6 & ~GPIO_CRL_MODE7;
	GPIOB->CRH |= GPIO_CRH_MODE8;
	GPIOB->CRH &= ~GPIO_CRH_CNF8;
	
	//disable steppers by defualt
	GPIOA->BSRR |= GPIO_BSRR_BS7;
	GPIOA->BSRR |= GPIO_BSRR_BS10;
	GPIOA->BSRR |= GPIO_BSRR_BS13;
	
}


/*******************************************
*	Function: moveX
*	Date: February 20, 2020
*	Purpose: Moves the X axis the desired amount
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveX(int movePosition){
	sendbyte(' ');
	sendbyte('X');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotor(1,moveAmount);
	
}

/*******************************************
*	Function: moveY
*	Date: February 20, 2020
*	Purpose: Moves the Y axis the desired amount
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveY(int movePosition){
	sendbyte(' ');
	sendbyte('Y');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotor(2,moveAmount);
}

/*******************************************
*	Function: moveZ
*	Date: February 20, 2020
*	Purpose: Moves the Z axis the desired amount
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveZ(int movePosition){
	sendbyte(' ');
	sendbyte('Z');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotor(3,moveAmount);
}

/*******************************************
*	Function: moveXSlow
*	Date: February 20, 2020
*	Purpose: Moves the X axis the desired amount at a slower speed
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveXSlow(int movePosition){
	sendbyte(' ');
	sendbyte('X');
	sendbyte('S');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotorSlow(1,moveAmount);
}

/*******************************************
*	Function: moveYSlow
*	Date: February 20, 2020
*	Purpose: Moves the Y axis the desired amount at a slower speed
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveYSlow(int movePosition){
	sendbyte(' ');
	sendbyte('Y');
	sendbyte('S');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotorSlow(2,moveAmount);
}

/*******************************************
*	Function: moveZSlow
*	Date: February 20, 2020
*	Purpose: Moves the Z axis the desired amount at a slower speed
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveZSlow(int movePosition){
	sendbyte(' ');
	sendbyte('Z');
	sendbyte('S');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	xPosG = movePosition;												//update the current position
	
	moveMotorSlow(3,moveAmount);
}

/*******************************************
*	Function: delayUs
*	Date: February 20, 2020
*	Purpose: Halts the CPU for the desired amount of time in micro seconds
*	Parameters: int uS
*	Return value: N/A
*******************************************/
void delayUs(int uS){
	int i = 0;
	int delay=0;
	delay = uS*6;									//factor of micro seconds to clock cycles
	for(i=0; i<delay; i++){				//sit in loop for delay duration
	}
	
}


/*******************************************
*	Function: moveMotor
*	Date: February 20, 2020
*	Purpose: Moves the motor the desired amount
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void moveMotor(int axis, int moveAmount){
	moveAmount = (moveAmount * stepsPerMM) / 10;				//convert move amount from milimeters to steps
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS14;
	}
	else{																								//if move direction is negative set direction outputs low
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR14;
		moveAmount = moveAmount * -1;											//if move direction is negative take the absolute value
	}
	printHex(moveAmount);
	sendbyte(' ');
	sendbyte(' ');
	
	
	if(moveAmount >= rampSize*2){												//if long move distance use accel, coast, devel ramp
		int speedAccel;
		int delayTime;
		
		//accel
		int accelRate = (maxSpeed - minSpeed) / rampSize;
		for(int i = 0; i < rampSize; i++){
			speedAccel = minSpeed + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
		//coast
		int coastSteps = moveAmount - rampSize*2;
		delayTime = (1000000/maxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
			
		}
		
		//decel
		for(int i = 0; i < rampSize; i++){
			speedAccel = maxSpeed -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
	}
	
	else{																									//if short move use accel, decel ramp
		int speedAccel;
		int delayTime;
		int maxSpeedReached;
		
		//accel
		int accelRate = (maxSpeed - minSpeed) / rampSize;
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = minSpeed + accelRate*(i);
			maxSpeedReached = speedAccel;
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
	}
	
}
	
	
	/*******************************************
*	Function: moveMotorSlow
*	Date: February 20, 2020
*	Purpose: Moves the motor the desired amount at a slower speed
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void moveMotorSlow(int axis, int moveAmount){
	moveAmount = (moveAmount * stepsPerMM) / 10;				//convert move amount from milimeters to steps
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS14;
	}
	else{																								//if move direction is negative set direction outputs low
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR14;
		moveAmount = moveAmount * -1;											//if move direction is negative take the absolute value
	}
	printHex(moveAmount);
	sendbyte(' ');
	sendbyte(' ');
	
	
	if(moveAmount >= slowRampSize*2){												//if long move distance use accel, coast, devel ramp
		int speedAccel;
		int delayTime;
		
		//accel
		int accelRate = (slowMaxSpeed - slowMinSpeed) / slowRampSize;
		for(int i = 0; i < slowRampSize; i++){
			speedAccel = slowMinSpeed + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
		//coast
		int coastSteps = moveAmount - slowRampSize*2;
		delayTime = (1000000/slowMaxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
			
		}
		
		//decel
		for(int i = 0; i < slowRampSize; i++){
			speedAccel = slowMaxSpeed -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
	}
	
	else{																									//if short move use accel, decel ramp
		int speedAccel;
		int delayTime;
		int maxSpeedReached;
		
		//accel
		int accelRate = (slowMaxSpeed - slowMinSpeed) / slowRampSize;
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = slowMinSpeed + accelRate*(i);
			maxSpeedReached = speedAccel;
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS15;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR9;
			else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR12;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR15;
			delayUs(delayTime);
		}
		
	}
	
	
	
}
/*******************************************
*	Function: enableMotors
*	Date: February 20, 2020
*	Purpose: Enables all stepper motors
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void enableMotors(void){
	GPIOA->BSRR |= GPIO_BSRR_BR7;
	GPIOA->BSRR |= GPIO_BSRR_BR10;
	GPIOA->BSRR |= GPIO_BSRR_BR13;
}

/*******************************************
*	Function: disableMotors
*	Date: February 20, 2020
*	Purpose: Disables all stepper motors
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void disableMotors(void){
	GPIOA->BSRR |= GPIO_BSRR_BS7;
	GPIOA->BSRR |= GPIO_BSRR_BS10;
	GPIOA->BSRR |= GPIO_BSRR_BS13;
}


/*
void delayUs(int uS){
	TIM3->CR1 |= TIM_CR1_CEN;
	myTicks = 0;
	while(myTicks<uS);
	
	TIM3->CR1 &= ~TIM_CR1_CEN;
}

void TIM3_IRQHandler(void){
	
	myTicks++;
	TIM3->SR &= ~TIM_SR_UIF;
}*/

