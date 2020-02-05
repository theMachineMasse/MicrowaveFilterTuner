/* Author - Jarett Tremblay, Jan 2019 */

#include <string.h>
#include "clocks.h"
#include "io.h"
#include "lcd.h"

char message1[30] = "  MFT 1.0.0 alpha";
char message2[30] = "  Status: Running";
char message3[30] = "  Tuning: Screw A";
char message4[30] = "  Position: (4,2)";

void msg2lcd(char *message)
{
	int i = 0;
	uint16_t messageLength = strlen(message);
	
	for(i=0;i<messageLength;i++)
	{
		dataToLCD(*message);
		++message;
	}
}

int main(void)
{
	
	clockInit();
	
	GPIOInit();
	
	outputInit();
	
	lcdInit();
	
	
	msg2lcd(message1);
	commandToLCD(0xc0);
	msg2lcd(message2);
	commandToLCD(0x80 | 0x14); // 0x14 address of 3rd line on LCD
	msg2lcd(message3);
	commandToLCD(0x80 | 0x54); // 0x54 address of 4th line on LCD
	msg2lcd(message4);
	
	
	while(1)
		
	{
		sw1LED();
	}
}

