void clockInit (void);
void serial_open (void);
void sendbyte(char);
char getbyte(void);
void command(void);
void clearBuffer(void);
void checkCommand(char input[30]);
void commandG0(void);
void commandG1(void);
void commandG28(void);
void commandM42(void);
void commandM17(void);
void commandM18(void);
void printHex(int hex);
int getNum(int);


