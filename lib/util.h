#include "global.h"


typedef struct builtIn
{
	const char *name;
	int (*func)(char *args[]);
} builtIn;

extern builtIn builtIns[];
extern const size_t NUM_BUILTIN;
void printPath(char *path, size_t size);
void parseArgs(char *args[], char inputBuff[]);
int runBuiltIn(char *args[]);
int builtInJcd(char *args[]);
int builtInJexit(char *args[]);
void handleSignal(int signal);
int readInput(struct termios *tsNew, struct termios *tsOld, char *inputBuffer, size_t bufferSize);