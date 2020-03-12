/********************************************
*	Project: Microwave Filter Tuner
* File: MotorControlSubSystem.h
*	Date: February 20, 2020
*	Programmer(s): Matthew Rostad and Jarett Tremblay
* Sub-System: Motor Control Sub-System
*	Description: Function declarations for all functions related to the Motor Control Sub-System (MCS)
* Version: 1.0
*	
********************************************/

/* FUNCTIONS */
void motorInit(void);
void moveX(int);
void moveY(int);
void moveZ(int);
void moveP(int);
void moveN(int);
void moveXSlow(int);
void moveYSlow(int);
void moveZSlow(int);
void moveZScrew(int);
void moveMotor(int,int);
void moveMotorSlow(int,int);
void enableMotors(void);
void disableMotors(void);
void homeMotors(void);
void phiHome(void);
void moveMotorDeg(float);
void nonRecoverableError(void);
void delayUs(int uS);
