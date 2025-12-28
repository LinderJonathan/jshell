#include "global.h"


typedef struct builtIn {
	const char *name;
	int (*func)(char *args[]);
} builtIn;

extern builtIn builtIns[];
extern const int NUM_BUILTIN;

int builtInJcd(char *args[]);
int builtInJexit(char *args[]);
void parseArgs(char *args[], char inputBuff[]);