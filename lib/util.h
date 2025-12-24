#include "global.h"


typedef struct builtIn {
	const char *name;
	void (*func)(char *args[]);
} builtIn;

extern builtIn builtIns[];

void builtInJcd(char *args[]);
void parseArgs(char *args[], char inputBuff[]);