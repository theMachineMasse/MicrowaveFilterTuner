/********************************************
*	Project: Microwave Filter Tuner
* File: Communications.h
*	Date: February 8, 2020
*	Programmer(s): Matthew Rostad and Jarett Tremblay
* Sub-System: Motor Control Sub-System and Visual Identification Sub-System
*	Description: Function declarations for all communication functions related to the Motor Control Sub-System (MCS) and Visual Identification Sub-System (VIS)
* Version: 1.0
*	
********************************************/

/* FUNCTIONS */
void clockInit (void);
void serial_open (void);
void sendbyte(char);
char getbyte(void);
void command(void);
void clearBuffer(void);
void checkCommand(char input[30]);
void commandG0(void);
void commandG1(void);
void commandG15(void);
void commandG16(void);
void commandG28(void);
void commandG30(void);
void commandM42(void);
void commandM17(void);
void commandM18(void);
void commandA(void);
void printHex(int hex);
int getNum(int);


