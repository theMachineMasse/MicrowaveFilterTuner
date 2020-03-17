/********************************************
*	Project: Microwave Filter Tuner
* File: MotorControlSubSystem.c
*	Date: February 20, 2020
*	Programmer(s): Matthew Rostad and Jarett Tremblay
* Sub-System: Motor Control Sub-System
*	Description: Contains the movement of the motors from Gcode commands
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "MotorControlSubSystem.h"
#include "Communications.h"
#include "stm32f10x.h"
#include "VisualDisplaySubSystem.h"

/* Globals */
int motorsEnabledG = 0;
int xPosG = 0;
int yPosG = 0;
int zPosG = 0;
const int stepsPerMM = 100;
const int rampSize = 1900;
const int maxSpeed = 8000;
const int minSpeed = 200;

const int homeRampSize = 1900;
const int homeMaxSpeed = 4000;
const int homeMinSpeed = 200;

const int slowRampSize = 190;
const int slowMaxSpeed = 800;
const int slowMinSpeed = 40;

int pDegG = 0;
int encFlag = 0;
const float stepsPerDeg = 8.88;	//steps per degree = 
const float rampSizeDeg = 1900;
const float maxSpeedDeg = 4000;
const float minSpeedDeg = 2000;
const float slowRampSizeDeg = 190;
const float slowMaxSpeedDeg = 400;
const float slowMinSpeedDeg = 40;

//Z axis hitting screw
const int depthSlow = 80;	//percentage of depth to move down until slowing down
const int depthMax = 150;	//percentage of depth to move down past where screw should have been


/*******************************************
*	Function: motorInit
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Initializes necessary motor GPIO pins
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void motorInit(void){	
	
	
	//remap pins
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	
	//motor GPIO init
	GPIOA->CRL |= GPIO_CRL_MODE7 ;
	GPIOA->CRL &= ~GPIO_CRL_CNF7 ;
	GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE15;
	GPIOA->CRH &= ~GPIO_CRH_CNF8 & ~GPIO_CRH_CNF9 & ~GPIO_CRH_CNF10 & ~GPIO_CRH_CNF11 & ~GPIO_CRH_CNF12 & ~GPIO_CRH_CNF15;
	
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
	GPIOB->CRL &= ~GPIO_CRL_CNF6 & ~GPIO_CRL_CNF7;
	GPIOB->CRH |= GPIO_CRH_MODE8;
	GPIOB->CRH &= ~GPIO_CRH_CNF8;
	
	
	//disable steppers by defualt
	disableMotors();
	
}


/*******************************************
*	Function: encoderInit
* Programmer(s): Jarett Tremblay
*	Date: February 25, 2020
*	Purpose: Initializes necessary encoder GPIO pins
*	Parameters: N/A
*	Return value: N/A
*******************************************/

void encoderInit(void){	
	
	// encoder GPIO init
	GPIOB->CRH |= GPIO_CRH_CNF8_0;
	GPIOB->CRH &= ~GPIO_CRH_MODE8 & ~GPIO_CRH_CNF8_1;  
	
}

/*******************************************
*	Function: moveX
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Moves the X axis the desired amount
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveX(int movePosition){
	sendbyte(' ');
	sendbyte('X');
	sendbyte(' ');
	lcdDisplayStatus(5);
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		xPosG = movePosition;												//update the current position
	}
	
	moveMotor(1,moveAmount);
	
}

/*******************************************
*	Function: moveY
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Moves the Y axis the desired amount
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveY(int movePosition){
	sendbyte(' ');
	sendbyte('Y');
	sendbyte(' ');
	lcdDisplayStatus(6);
	int moveAmount = movePosition - yPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		yPosG = movePosition;												//update the current position
	}
	
	moveMotor(2,moveAmount);
}

/*******************************************
*	Function: moveZ
*	Programmer(s): Matthew Rostad
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
	lcdDisplayStatus(7);
	int moveAmount = movePosition - zPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		zPosG = movePosition;												//update the current position
	}
	
	moveMotor(3,moveAmount);
}

/*******************************************
*	Function: moveXSlow
*	Programmer(s): Matthew Rostad
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
	lcdDisplayStatus(5);
	int moveAmount = movePosition - xPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		xPosG = movePosition;												//update the current position
	}
	
	moveMotorSlow(1,moveAmount);
}

/*******************************************
*	Function: moveYSlow
*	Programmer(s): Matthew Rostad
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
	lcdDisplayStatus(6);
	int moveAmount = movePosition - yPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		yPosG = movePosition;												//update the current position
	}
	
	moveMotorSlow(2,moveAmount);
}

/*******************************************
*	Function: moveZSlow
*	Programmer(s): Matthew Rostad
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
	lcdDisplayStatus(7);
	int moveAmount = movePosition - zPosG;			//amount to move based off desired position minus current position 
	if(motorsEnabledG){
		zPosG = movePosition;												//update the current position
	}
	
	moveMotorSlow(3,moveAmount);
}


/*******************************************
*	Function: moveP
*	Programmer(s): Matthew Rostad and Jarett Tremblay
*	Date: February 25, 2020
*	Purpose: Rotates the Phi axis the desired amount in degrees
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveP(int movePosition){
	sendbyte(' ');
	sendbyte('P');
	printHex(movePosition);
	sendbyte(' '); 
	pDegG = movePosition;												//update the current position
	
	moveMotorDeg(movePosition);
	
}

/*******************************************
*	Function: moveN
*	Programmer(s): Matthew Rostad and Jarett Tremblay
*	Date: February 25, 2020
*	Purpose: Rotates the Phi axis opposite to moveP
*	Parameters: int movePosition
*	Return value: N/A
*******************************************/
void moveN(int movePosition){
	sendbyte(' ');
	sendbyte('N');
	printHex(movePosition);
	sendbyte(' '); 
	pDegG = movePosition;												//update the current position
	
	moveMotorDeg(-movePosition);
	
}




/*******************************************
*	Function: delayUs
*	Programmer(s): Matthew Rostad
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
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Moves the motor the desired amount
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void moveMotor(int axis, int moveAmount){
	moveAmount = (moveAmount * stepsPerMM) / 10;				//convert move amount from milimeters to steps
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS10;
	}
	else{																								//if move direction is negative set direction outputs low
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR10;
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
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		//coast
		int coastSteps = moveAmount - rampSize*2;
		delayTime = (1000000/maxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		//decel
		for(int i = 0; i < rampSize; i++){
			speedAccel = maxSpeed -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
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
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
	}
	
}
	
	
/*******************************************
*	Function: moveMotorSlow
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Moves the motor the desired amount at a slower speed
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void moveMotorSlow(int axis, int moveAmount){
	moveAmount = (moveAmount * stepsPerMM) / 10;				//convert move amount from milimeters to steps
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS10;
	}
	else{																								//if move direction is negative set direction outputs low
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR10;
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
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		//coast
		int coastSteps = moveAmount - slowRampSize*2;
		delayTime = (1000000/slowMaxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		//decel
		for(int i = 0; i < slowRampSize; i++){
			speedAccel = slowMaxSpeed -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
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
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			//check for limit switch hit
			if((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1){ lcdDisplayError(25); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2){ lcdDisplayError(26); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3){ lcdDisplayError(24); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4){ lcdDisplayError(23); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5){ lcdDisplayError(27); nonRecoverableError();}
			if((GPIOA->IDR & GPIO_IDR_IDR6) == GPIO_IDR_IDR6){ lcdDisplayError(28); nonRecoverableError();}
		}
		
	}
	
	
	
}




/*******************************************
*	Function: moveZScrew
*	Programmer(s): Matthew Rostad
*	Date: March 09, 2020
*	Purpose: Moves the z axis to align with screw
*	Parameters: int depth
*	Return value: N/A
*******************************************/
void moveZScrew(int depth){
	int delayTime;
	int speedAccel;
	depth = (depth * stepsPerMM) / 10;
	int fastDepth = (depth * depthSlow) / 100;
	int maxDepth = (depth * depthMax) / 100;
	int zSteps = 0;

	GPIOA->BSRR |= GPIO_BSRR_BS10;	//move away from home
	
	
	//accel
	int accelRate = (homeMaxSpeed - homeMinSpeed) / homeRampSize;
	for(int i = 0; i < homeRampSize; i++){
			speedAccel = homeMinSpeed + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			zSteps++;
			if((GPIOC->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10){ zPosG = (zSteps / stepsPerMM)*10; return;}
	}
	                                                                       
	//coast
		int coastSteps = fastDepth - homeRampSize;
		delayTime = (1000000/homeMaxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			zSteps++;
			if((GPIOC->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) { zPosG = (zSteps / stepsPerMM)*10; return;}
		}
		
		
	//coast at slow speed
		coastSteps = maxDepth - fastDepth;
		delayTime = (1000000/slowMaxSpeed)/2;
		for(int i = 0; i < coastSteps; i++){
			GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			zSteps++;
			if((GPIOC->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) { zPosG = (zSteps / stepsPerMM)*10; return;}
		}
		
		zPosG = (zSteps / stepsPerMM)*10;
		lcdDisplayError(22);	//no screw detected
}
	
	
	
/*******************************************
*	Function: enableMotors
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Enables all stepper motors
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void enableMotors(void){
	GPIOB->BSRR |= GPIO_BSRR_BR6;
	GPIOA->BSRR |= GPIO_BSRR_BR7;
	GPIOA->BSRR |= GPIO_BSRR_BR10;
	GPIOA->BSRR |= GPIO_BSRR_BR13;
	motorsEnabledG = 1;
}

/*******************************************
*	Function: disableMotors
*	Programmer(s): Matthew Rostad
*	Date: February 20, 2020
*	Purpose: Disables all stepper motors
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void disableMotors(void){
	GPIOB->BSRR |= GPIO_BSRR_BS6;
	GPIOA->BSRR |= GPIO_BSRR_BS7;
	GPIOA->BSRR |= GPIO_BSRR_BS10;
	GPIOA->BSRR |= GPIO_BSRR_BS13;
	motorsEnabledG = 0;
}


/*******************************************
*	Function: homeMotors
*	Programmer(s): Matthew Rostad
*	Date: February 29, 2020
*	Purpose: Home X, Y and Z motors
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void homeMotors(void){
 if(motorsEnabledG){	
	lcdDisplayStatus(3);
	int delayTime;
	int speedAccel;
	int homeTimeout = 1000000;
	int timeoutCounter = 0;
	int axis;
	

	for(int i = 0; i < 3; i++){
		if(i == 0) axis = 3;	//z axis
		if(i == 1) axis = 1;	//x axis
		if(i == 2) axis = 2;	//y axis
		timeoutCounter = 0;
		//accel
		//move axis towards home
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR10;
		int accelRate = (homeMaxSpeed - homeMinSpeed) / homeRampSize;
		for(int i = 0; i < homeRampSize; i++){
			speedAccel = homeMinSpeed + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			
			//if(((GPIOA->IDR & GPIO_IDR_IDR1) != GPIO_IDR_IDR1) && ((GPIOA->IDR & GPIO_IDR_IDR3) != GPIO_IDR_IDR3) && ((GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5)){
			if(((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1) || ((GPIOA->IDR & GPIO_IDR_IDR3) == GPIO_IDR_IDR3) || ((GPIOA->IDR & GPIO_IDR_IDR5) == GPIO_IDR_IDR5)){
				break;
			}
			}
		
		//coast
		delayTime = (1000000/homeMaxSpeed)/2;
		while(((GPIOA->IDR & GPIO_IDR_IDR1) != GPIO_IDR_IDR1) && ((GPIOA->IDR & GPIO_IDR_IDR3) != GPIO_IDR_IDR3) && ((GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5)){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
			timeoutCounter ++;
			if(timeoutCounter > homeTimeout){
				if     (axis == 1) lcdDisplayError(14);
				else if(axis == 2) lcdDisplayError(15);
				else if(axis == 3) lcdDisplayError(16);
				break;
				}
		}
		
		//slowly move away from the limit switch
		//move axis away from home
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS10;
		
		delayTime = (1000000/slowMaxSpeed)/2;
		for(int i = 0; i < 500; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
		}
		
		//slowly move towards limit switch until contact
		//move axis towards home
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR10;
		delayTime = (1000000/slowMaxSpeed)/2;
		while(((GPIOA->IDR & GPIO_IDR_IDR1) != GPIO_IDR_IDR1) && ((GPIOA->IDR & GPIO_IDR_IDR3) != GPIO_IDR_IDR3) && ((GPIOA->IDR & GPIO_IDR_IDR5) != GPIO_IDR_IDR5)){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
		}
		
		//slowly move away from the limit switch
		//move axis away from home
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS12;
		else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS7;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS10;
		
		delayTime = (1000000/slowMaxSpeed)/2;
		for(int i = 0; i < 250; i++){
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BS8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS11;
			delayUs(delayTime);
			if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR15;
			else if(axis == 2) GPIOB->BSRR |= GPIO_BSRR_BR8;
			else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR11;
			delayUs(delayTime);
		}
		if     (axis == 1) xPosG = 0;
		else if(axis == 2) yPosG = 0;
		else if(axis == 3) zPosG = 0;
}
}
}

/*******************************************
*	Function: moveMotorDeg
* Programmer(s): Jarett Tremblay
*	Date: February 20, 2020
*	Purpose: Moves the motor the desired amount in degrees
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void moveMotorDeg(float moveAmount){
	sendbyte('v');
	
	lcdDisplayStatus(8);																//moving phi status
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		GPIOA->BSRR |= GPIO_BSRR_BR8;
	}
	else{																								//if move direction is negative set direction outputs low
		GPIOA->BSRR |= GPIO_BSRR_BS8;
		moveAmount = moveAmount * -1;											//if move direction is negative take the absolute value
	}
	

	moveAmount = (moveAmount * stepsPerDeg) / 10;				//convert move amount from degrees to steps
	
	moveAmount = (int)moveAmount;
	
	printHex(moveAmount);
	sendbyte(' ');
	sendbyte(' ');
	
	
	if(moveAmount >= rampSizeDeg*2){												//if long move distance use accel, coast, devel ramp
		int speedAccel;
		int delayTime;
		
		//accel
		int accelRate = (maxSpeedDeg - minSpeedDeg) / rampSizeDeg;
		for(int i = 0; i < rampSizeDeg; i++){
			speedAccel = minSpeedDeg + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			GPIOA->BSRR |= GPIO_BSRR_BS9;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR9;
			delayUs(delayTime);
		}
		
		//coast
		int coastSteps = moveAmount - rampSizeDeg*2;
		delayTime = (1000000/maxSpeedDeg)/2;
		for(int i = 0; i < coastSteps; i++){
			GPIOA->BSRR |= GPIO_BSRR_BS9;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR9;
			delayUs(delayTime);
			
		}
		
		//decel
		for(int i = 0; i < rampSizeDeg; i++){
			speedAccel = maxSpeedDeg -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			GPIOA->BSRR |= GPIO_BSRR_BS9;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR9;
			delayUs(delayTime);
		}
	}
	
	else{																									//if short move use accel, decel ramp
		int speedAccel;
		int delayTime;
		int maxSpeedReached;
		
		//accel
		int accelRate = (maxSpeedDeg - minSpeedDeg) / rampSizeDeg;
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = minSpeedDeg + accelRate*(i);
			maxSpeedReached = speedAccel;
			delayTime = (1000000/speedAccel)/2;
			
			GPIOA->BSRR |= GPIO_BSRR_BS9;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR9;
			delayUs(delayTime);
		}
		
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			GPIOA->BSRR |= GPIO_BSRR_BS9;
			delayUs(delayTime);
			GPIOA->BSRR |= GPIO_BSRR_BR9;
			delayUs(delayTime);
		}
	}
	
	//lcdDisplayStatus(2);																//ready status																	
	
}

/*******************************************
*	Function: phiHome
* Programmer(s): Jarett Tremblay
*	Date: February 29, 2020
*	Purpose: Moves the motor the desired amount in degrees
*	Parameters: int axis, int movePosition
*	Return value: N/A
*******************************************/
void phiHome(){
	
	int moveAmount = 360; 															//move one full rotation
	int timeoutValue = 12960;														//(moveAmount * stepsPerDeg) / 10
	int timeoutCount = 0;																//timeout counter
		
	
	if(moveAmount > 0){																	//if move direction is positive set direction outputs high
		GPIOA->BSRR |= GPIO_BSRR_BS8;
	}
	else{																								//if move direction is negative set direction outputs low
		GPIOA->BSRR |= GPIO_BSRR_BR8;
		moveAmount = moveAmount * -1;											//if move direction is negative take the absolute value
	}
	
	moveAmount = (moveAmount * stepsPerDeg) / 10;				//convert move amount from degrees to steps
	
	printHex(moveAmount);
	sendbyte(' ');
	sendbyte(' ');
	
	lcdDisplayStatus(4);															  //status: homing phi
	
	if(moveAmount <= rampSizeDeg*2){										//if long move distance use accel, coast, devel ramp
		int speedAccel;
		int delayTime;
		
		//accel
		int accelRate = (maxSpeedDeg - minSpeedDeg) / rampSizeDeg;
		for(int i = 0; i < rampSizeDeg; i++){
			speedAccel = minSpeedDeg + accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
			
				GPIOA->BSRR |= GPIO_BSRR_BS9;
				
				timeoutCount++;
	
				if(timeoutCount >= timeoutValue){
					lcdDisplayError(13);
				}
								
				delayUs(delayTime);
				
				if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
					
					GPIOA->BSRR |= GPIO_BSRR_BR9;
					
					timeoutCount++;
	
					if(timeoutCount >= timeoutValue){
						lcdDisplayError(13);
					}
					
					delayUs(delayTime);
				}
				else{
					i = rampSizeDeg;
				}
			}
			else{
				i = rampSizeDeg;
			}
		}
		
		//coast
		int coastSteps = moveAmount - rampSizeDeg*2;
		delayTime = (1000000/maxSpeedDeg)/2;
		for(int i = 0; i < coastSteps; i++){
			
			if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
				
				GPIOA->BSRR |= GPIO_BSRR_BS9;
				
				timeoutCount++;
	
				if(timeoutCount >= timeoutValue){
					lcdDisplayError(13);
				}
					
				delayUs(delayTime);
				
				if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
					
					GPIOA->BSRR |= GPIO_BSRR_BR9;
					
					timeoutCount++;
	
					if(timeoutCount >= timeoutValue){
						lcdDisplayError(13);
					}
					
					delayUs(delayTime);
				}
				else{
					i = coastSteps;
				}
			}
			else{
				i = coastSteps;
			}
		}
		
		//decel
		for(int i = 0; i < rampSizeDeg; i++){
			speedAccel = maxSpeedDeg - accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
				
				GPIOA->BSRR |= GPIO_BSRR_BS9;
				
				timeoutCount++;
	
				if(timeoutCount >= timeoutValue){
					lcdDisplayError(13);
				}
					
				delayUs(delayTime);
				
				if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
				
					GPIOA->BSRR |= GPIO_BSRR_BR9;
					
					timeoutCount++;
	
					if(timeoutCount >= timeoutValue){
						lcdDisplayError(13);
					}
					
					delayUs(delayTime);
				}
				else{
					i = rampSizeDeg;
				}
			}
			else{
				i = rampSizeDeg;
			}
		}
	}
	
	else{																									//if short move use accel, decel ramp
		int speedAccel;
		int delayTime;
		int maxSpeedReached;
		
		//accel
		int accelRate = (maxSpeedDeg - minSpeedDeg) / rampSizeDeg;
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = minSpeedDeg + accelRate*(i);
			maxSpeedReached = speedAccel;
			delayTime = (1000000/speedAccel)/2;
			
			if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
				
				GPIOA->BSRR |= GPIO_BSRR_BS9;
				
				timeoutCount++;
	
				if(timeoutCount >= timeoutValue){
					lcdDisplayError(13);
				}
					
				delayUs(delayTime);
				
				if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
					
					GPIOA->BSRR |= GPIO_BSRR_BR9;
					timeoutCount++;
	
					if(timeoutCount >= timeoutValue){
						lcdDisplayError(13);
					}
					
					delayUs(delayTime);
				}
				else{
					i = moveAmount/2;
				}
			}
			else{
			i = moveAmount/2;
			}			
		}
		
		//decel
		for(int i = 0; i < moveAmount/2; i++){
			speedAccel = maxSpeedReached -  accelRate*(i);
			delayTime = (1000000/speedAccel)/2;
			
			if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
				
				GPIOA->BSRR |= GPIO_BSRR_BS9;
				
				timeoutCount++;
	
				if(timeoutCount >= timeoutValue){
					lcdDisplayError(13);
				}
					
				delayUs(delayTime);
				
				if ((GPIOB->IDR & GPIO_IDR_IDR9) == 0){
					
					GPIOA->BSRR |= GPIO_BSRR_BR9;
					
					timeoutCount++;
	
					if(timeoutCount >= timeoutValue){
						lcdDisplayError(13);
					}
					
					delayUs(delayTime);
				}
				else{
					i = moveAmount/2;
				}
			}
			else{
				i = moveAmount/2;
			}			
		}
	}
	
	//lcdDisplayStatus(2);																//status: ready
	
}


/*******************************************
*	Function: nonRecoverableError
*	Programmer(s): Matthew Rostad
*	Date: March 1, 2020
*	Purpose: Inifinite loop for when a non recoverable error occurs
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void nonRecoverableError(){
	while(1);
}





