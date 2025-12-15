#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

#define ANSI_COLOR_BLUE "\033[38;5;27m"
#define ANSI_COLOR_VIOLET "\033[38;5;135m"
#define ANSI_COLOR_RESET "\033[0m"

#define MAX_INPUT_SIZE      256
#define MAX_NUM_OF_ARGS     64
#define MAX_PATH_SIZE       256

void parseArgs(char *args[], char inputBuff[]);