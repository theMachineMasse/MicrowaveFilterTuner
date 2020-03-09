/********************************************
*	Project: Microwave Filter Tuner
* File: Communications.c
*	Date: January 27, 2020
*	Programmer(s): Matthew Rostad
* Sub-System: Motor Control Sub-System
*	Description: Contains all the functions to send and receive G code commands from the PC
* Version: 1.0
*	
********************************************/

/* INCLUDES */
#include "stm32f10x.h"
#include "Communications.h"
#include "MotorControlSubSystem.h"
#include "VisualIdentificationSubSystem.h"
#include "VisualDisplaySubSystem.h"

//Globals
char get[30];				//CLI character buffer
char input;				//temp value to store input value


/*******************************************
*	Function: clockInit
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: Initializes necessary clock registers
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void clockInit(void)
{
		uint32_t temp = 0x00;
    //If you hover over the RCC you can go to the definition and then
    //see it is a structure of all the RCC registers.  Then you can
    //simply assign a value.
    RCC->CFGR = 0x00050002;     // Output PLL/2 as MCO,
                                // PLLMUL X3, PREDIV1 is PLL input

    RCC->CR =  0x01010081;      // Turn on PLL, HSE, HSI

    while (temp != 0x02000000)  // Wait for the PLL to stabilize
    {
        temp = RCC->CR & 0x02000000; //Check to see if the PLL lock bit is set
    }

		
	  //Enable peripheral clocks for various ports and subsystems
    //Bit 4: Port C Bit3: Port B Bit 2: Port A
    RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN
        | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN;
		
		RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN
        | RCC_APB2ENR_IOPAEN ;
		
	//enable UART clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		
}

/*******************************************
*	Function: serial_open
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: Initializes necessary serial registers and begins serial communication
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void serial_open (void)
{

	
	//set PB10 as AFIO push pull output	
  GPIOB->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE10_1;
  GPIOB->CRH &= ~GPIO_CRH_CNF10_0 ;
	
	USART3->CR1 |= USART_CR1_UE;
	USART3->BRR = 0x9C4;
	USART3->CR1 |= USART_CR1_TE;
	USART3->CR1 |= USART_CR1_RE;
}


/*******************************************
*	Function: sendbyte
*	Programmer(s): Matthew Rostad
*	Date: October 7,2019
*	Purpose: Sends a byte to serial interface
*	Parameters: data
*	Return value: N/A
*******************************************/
void sendbyte(char BYTE)
{
	while((USART_SR_TXE & USART3->SR) != USART_SR_TXE)
	{
	}
	
	USART3->DR = BYTE;
}


/*******************************************
*	Function: getbyte
*	Programmer(s): Matthew Rostad
*	Date: October 7,2019
*	Purpose: Receives byte from serial interface
*	Parameters: data
*	Return value: N/A
*******************************************/
char getbyte(void)
{
	while((USART_SR_RXNE & USART3->SR) != USART_SR_RXNE)
	{

	}
	
	return USART3->DR;
	
}


/*******************************************
*	Function: clearBuffer
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: cleans 30 character buffer
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void clearBuffer(void) {
	int i =0;
	//put null in all buffer spots
	for (i=0;i<sizeof(get);i++){
		get[i] = '\0';
	}
}

/*******************************************
*	Function: command
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: Logic to interperate input from Gcode commands
*	Parameters: N/A
*	Return value: N/A
*******************************************/


int i = 0;				//counter to know where in character array buffer
void command(void){
	clearBuffer();
	while(1){
		input = getbyte();		//get the current byte
		get[i] = input;				//set the temperary char to the current byte
		lcdDisplayStatus(2);	//set ready status
		
		if((input == 0xD) || (i >= 29)){	//if the input is an enter command check the byte
			checkCommand(get);	//check to see if command matches an input command
			
			clearBuffer();			//clear the character buffer
			i = 0;							//reset the character counter to zero
		}
		else{
			sendbyte(input);
			i++;
		}
		
	}
}


/*******************************************
*	Function: checkCommand
*	Programmer(s): Matthew Rostad
*	Date: October 7, 2019
*	Purpose: Determines which command is to be ran
*	Parameters: input[]
*	Return value: N/A
*******************************************/
void checkCommand(char input[30]){
	
	if ((get[0] == 'G' || get[0] == 'g') && (get[1] == '0')){
		commandG0();
	}
	
	else if ((get[0] == 'G'|| get[0] == 'g') && (get[1] == '1') && (get[2] == ' ')){
		commandG1();
	}
	
	
	else if ((get[0] == 'G'|| get[0] == 'g') && (get[1] == '2') && (get[2] == '8')){
		commandG28();
	}
	
	else if ((get[0] == 'G'|| get[0] == 'g') && (get[1] == '3') && (get[2] == '0')){
		commandG30();
	}
	
	else if ((get[0] == 'G'|| get[0] == 'g') && (get[1] == '1') && (get[2] == '5')){
		commandG15();
	}
	
	else if ((get[0] == 'G'|| get[0] == 'g') && (get[1] == '1') && (get[2] == '6')){
		sendbyte('q');
		commandG16();
	}
	
	else if ((get[0] == 'M'|| get[0] == 'm') && (get[1] == '4') && (get[2] == '2')){
		commandM42();
	}
	
	else if ((get[0] == 'M'|| get[0] == 'm') && (get[1] == '1') && (get[2] == '7')){
		commandM17();
	}
	
	else if ((get[0] == 'M'|| get[0] == 'm') && (get[1] == '1') && (get[2] == '8')){
		commandM18();
	}

	
	
	
	
	
	
}




/*******************************************
*	Function: commandG0
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the G1 command to move the machine axis at full speed
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG0(void){
	for(int i=2; i<30; i++){
		if(get[i] == ' '){
		}
		else if(get[i] == 'X' || get[i] == 'x'){
			moveX(getNum(i+1));
		}
		else if(get[i] == 'Y'|| get[i] == 'y'){
			moveY(getNum(i+1));
		}
		else if(get[i] == 'Z'|| get[i] == 'z'){
			moveZ(getNum(i+1));
		}
		else{
		}
	}
	
}


/*******************************************
*	Function: commandG1
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the G1 command to move the machine axis slowly
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG1(void){
		for(int i=2; i<30; i++){
		if(get[i] == ' '){
		}
		else if(get[i] == 'X' || get[i] == 'x'){
			moveXSlow(getNum(i+1));
		}
		else if(get[i] == 'Y'|| get[i] == 'y'){
			moveYSlow(getNum(i+1));
		}
		else if(get[i] == 'Z'|| get[i] == 'z'){
			moveZSlow(getNum(i+1));
		}
		else{
		}
	}
}



/*******************************************
*	Function: commandG15
*	Programmer(s): Jarett Tremblay and Matthew Rostad
*	Date: February 25, 2020
*	Purpose: interprets the G15 command to home the phi axis
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG15(void){
	  phiHome();
}



/*******************************************
*	Function: commandG16
*	Programmer(s): Jarett Tremblay
*	Date: February 25, 2020
*	Purpose: interprets the G16 command to move the machine phi axis at full speed
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG16(void){
	  sendbyte('P');
		for(int i=2; i<30; i++){
		if(get[i] == ' '){
		}
		else if(get[i] == 'P' || get[i] == 'p'){
			moveP(getNum(i+1));
			}
		else if(get[i] == 'N' || get[i] == 'n'){
			moveN(getNum(i+1));
			}
		else{
		}
	}
}


/*******************************************
*	Function: printHex
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: converts hex to ascii code and prints to screen
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void printHex(int hex){
	int hex_temp;
	
	hex_temp = (hex&0xF000)>>12;
	if(hex_temp <= 9)
		sendbyte(hex_temp + 0x30);
	else
		sendbyte(hex_temp + 0x37);
	
	hex_temp = (hex&0xF00)>>8;
	if(hex_temp <= 9)
		sendbyte(hex_temp + 0x30);
	else
		sendbyte(hex_temp + 0x37);
	
	hex_temp = (hex&0xF0)>>4;
	if(hex_temp <= 9)
		sendbyte(hex_temp + 0x30);
	else
		sendbyte(hex_temp + 0x37);
	
	hex_temp = hex&0xF;
	if(hex_temp <= 9)
		sendbyte(hex_temp + 0x30);
	else
		sendbyte(hex_temp + 0x37);
	
		
}


/*******************************************
*	Function: getNum
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: determines the number being entered after a move command
*	Parameters: int
*	Return value: int
*******************************************/
int getNum(int i){
	int moveNum[6] = {0,0,0,0,0,0};
	int length = 0;
	int decimal = 0;
	int moveAmount = 0;
	int multiplyFactor = 1;
	int negative = 1;
	for(int j = 0; j<6; j++){
		if(get[j+i] == ' '){ 	//if character is a space the number is over. Exit the loop
			j = 7;							//exit the for loop
			length ++;
		}
		else if(get[j+i] == '\0'){	//if character is a space the number is over. Exit the loop
			j = 7;										//exit the for loop
		}
		
		else if(get[j+1] == '.' || (get[j+i] >= 48 && get[j+i] <= 57)){	//only accept character if get is a number of a decimal point
			moveNum[j] = get[j+i];
			length = j;
		}
	}
	
	
	for(int j=0; j < length; j++){
		if(moveNum[j] == '.'){
			decimal = j;
		}
		else{
		moveNum[j] = moveNum[j] - 48;
		}
	}
	
	
	if(decimal == 0){
		for(int j = length-1; j >= 0; j--){
			multiplyFactor = multiplyFactor * 10;
			moveAmount = moveAmount + moveNum[j] * multiplyFactor;
		}
	}
	else{
		for(int j = length-1; j >= 0; j--){
			if(moveNum[j] != '.'){
				moveAmount = moveAmount + moveNum[j] * multiplyFactor;
				multiplyFactor = multiplyFactor * 10;
			}
				
		}
	}
	
	printHex(moveAmount);
	moveAmount = moveAmount*negative;
	printHex(moveAmount);
	
	return moveAmount;
}


/*******************************************
*	Function: commandG28
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the G28 command to send the machine home
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG28(void){
	sendbyte('H');
	homeMotors();
}


/*******************************************
*	Function: commandG30
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the G28 command to move z axis to hit screw
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandG30(void){
		for(int i=2; i<30; i++){
		if(get[i] == ' '){
		}
		else if(get[i] == 'Z'|| get[i] == 'z'){
			moveZScrew(getNum(i+1));
		}
		else{
		}
	}
}


/*******************************************
*	Function: commandM42
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the M42 command to turn on or off a specific I/O pin
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandM42(void){
	int pin = 0;
	int value = 0;
	for(int i=3; i<30; i++){
		if(get[i] == 'P' || get[i] == 'p'){
			pin = get[i+1] - 48;
		}
		if(get[i] == 'S' || get[i] == 's'){
			value = get[i+1] - 48;
		}
	}
	
	lightsToggle(pin, value);

	
}


/*******************************************
*	Function: commandM17
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets theM17 command to enable all steppers
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandM17(void){
	enableMotors();
	
}


/*******************************************
*	Function: commandM18
*	Programmer(s): Matthew Rostad
*	Date: February 8, 2020
*	Purpose: interprets the M18 command to disable all steppers
*	Parameters: N/A
*	Return value: N/A
*******************************************/
void commandM18(void){
	disableMotors();
	
}
