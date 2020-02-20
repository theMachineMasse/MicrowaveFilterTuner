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



void motorInit(void){	
	//motor clock init
  RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN| RCC_APB2ENR_IOPAEN ;
	
	
	
	
	//motor GPIO init
	GPIOA->CRL |= GPIO_CRL_MODE7 ;
	GPIOA->CRL &= ~GPIO_CRL_CNF7 ;
	GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15;
	GPIOA->CRH &= ~GPIO_CRH_CNF8 & ~GPIO_CRH_CNF9 & ~GPIO_CRH_CNF10 & ~GPIO_CRH_CNF11 & ~GPIO_CRH_CNF12 & ~GPIO_CRH_CNF13 & ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF14 & ~GPIO_CRH_CNF15;
	
	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
	GPIOB->CRL &= ~GPIO_CRL_MODE6 & ~GPIO_CRL_MODE7;
	GPIOB->CRH |= GPIO_CRH_MODE8;
	GPIOB->CRH &= ~GPIO_CRH_CNF8;
	
	
	/*
	//timer init
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	TIM3->PSC = 0;
	TIM3->ARR = 1;								//1MHZ freq
	//TIM3->CR1 |= TIM_CR1_URS;			//generate interrupt at overflow
	//TIM3->DIER |= TIM_DIER_UIE;		//update interrupt enable
	TIM3->EGR |= TIM_EGR_UG;			//update generation
	TIM3->CR1 |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	*/
	
}



void moveX(int movePosition){
	sendbyte(' ');
	sendbyte('X');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;
	xPosG = movePosition;
	
	moveMotor(1,moveAmount);
	
}

void moveY(int movePosition){
	sendbyte(' ');
	sendbyte('Y');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;
	xPosG = movePosition;
	
	moveMotor(2,moveAmount);
}

void moveZ(int movePosition){
	sendbyte(' ');
	sendbyte('Z');
	printHex(movePosition);
	sendbyte(' ');
	int moveAmount = movePosition - xPosG;
	xPosG = movePosition;
	
	moveMotor(3,moveAmount);
}


void moveXSlow(int movePosition){
	sendbyte(' ');
	sendbyte('X');
	sendbyte('S');
	printHex(movePosition);
	sendbyte(' ');
}

void moveYSlow(int movePosition){
	sendbyte('Y');
	sendbyte('S');
	printHex(movePosition);
}

void moveZSlow(int movePosition){
	sendbyte('Z');
	sendbyte('S');
	printHex(movePosition);
}

void delayUs(int uS){
	int i = 0;
	int delay=0;
	delay = uS*6;
	for(i=0; i<delay; i++){
	}
	
}



void moveMotor(int axis, int moveAmount){
	moveAmount = (moveAmount * stepsPerMM) / 10;
	
	if(moveAmount > 0){
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BS8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BS11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BS14;
	}
	else{
		if     (axis == 1) GPIOA->BSRR |= GPIO_BSRR_BR8;
		else if(axis == 2) GPIOA->BSRR |= GPIO_BSRR_BR11;
		else if(axis == 3) GPIOA->BSRR |= GPIO_BSRR_BR14;
		moveAmount = moveAmount * -1;
	}
	printHex(moveAmount);
	
	
	if(moveAmount >= rampSize*2){
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
	
	else{
		sendbyte('b');
	}
	
	
	
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

