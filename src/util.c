#include "../lib/global.h"
#include "../lib/util.h"
#include "../lib/jshell.h"

builtIn builtIns[] = 
{
	{
		"jexit",
		builtInJexit
	},
	{
		"jcd",
		builtInJcd
	}
};

const size_t NUM_BUILTIN = sizeof(builtIns) / sizeof(builtIn);

// Clear 'bufferLength' amount of char from command line.
void clearLine(int bufferLength)
{
	for (int i = 0; i < bufferLength; i++)
	{
		write(STDOUT_FILENO, "\b \b", 3);
	}
}

void printPath(char *path, size_t size)
{

	if (getcwd(path, size) != NULL)
	{
		printf("%s > ", path);
	}
	fflush(stdout);
}

void parseArgs(char* args[], char inputBuff[])
{
	int i = 0;
	int k = 0;
	bool inArg = false;

	while (inputBuff[i] != '\0')
	{
		if (inputBuff[i] != ' ' && !inArg)
		{
			args[k] = &inputBuff[i];
			inArg = true;
			k++;
		}
		else if (inputBuff[i] == ' ' && inArg)
		{
			inputBuff[i] = '\0';
			inArg = false;
		}
		i++;
	}
	args[k] = NULL;
}

int runBuiltIn(char *args[])
{
	for (int i = 0; i < NUM_BUILTIN; i++)
	{
		if (strcmp(args[0], builtIns[i].name) == 0)
		{
			builtIns[i].func(args);
			return i;
		} 
	}
	return -1;
}

int builtInJcd(char *args[])
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "cd: no path provided\n");
		return 1;
	}
	if (chdir(args[1]) == -1)
	{
		perror("cd");
	}
	return 1;
}

int builtInJexit(char *args[])
{
	exit(EXIT_SUCCESS);
}


int readInput(struct termios *tsNew, struct termios *tsOld, char *inputBuffer, size_t bufferSize)
{
	memset(inputBuffer, 0, bufferSize);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, tsNew);
	int inputLength = 0;
	char c;
	while (inputLength < bufferSize - 1)
	{
		// if byte read and written to buffer
		if (read(STDIN_FILENO, &c, 1) == 1)
		{
			// handle enter
			if (c == '\n')
			{
				write(STDOUT_FILENO, "\n", 1);

				if (inputLength == 0) { break; }
				strcpy(commandHistory[commandHead], inputBuffer);
				// increment commandHead, set index where head is
				// If not, commandIndex would point at an old command; 
				commandHead = (commandHead + 1) % MAX_NUM_COMMAND_HISTORY;
				commandIndex = commandHead;
				if (commandLen < MAX_NUM_COMMAND_HISTORY) { commandLen++; }
				break;
			}

			// handle tab, depending on shell tab length
			else if (c == '\t')
			{
				for (int j = 0; j < SHELL_TAB_LENGTH && inputLength < bufferSize - 1; j++)
				{
					inputBuffer[inputLength] = ' ';
					inputLength++;
					write(STDOUT_FILENO, " ", 1);
				}
				continue;
			}
			// handle backspace
			else if (c == 0x7f || c == '\b')
			{
				if (inputLength > 0)
				{
					inputLength--;
					write(STDOUT_FILENO, "\b \b", 3);
				}
				continue;
			}
			// handle arrow up or down
			else if (c == '\x1b')
			{
				char seq[2];
				read(STDIN_FILENO, &seq[0], 1);
				read(STDIN_FILENO, &seq[1], 1);
				if (seq[0] == '[')
				{
					// handle UP
					if (seq[1] == 'A')
					{
						if (commandLen == 0) { return 0; }
						commandIndex = (commandIndex + 1) % commandLen;
						
					}
					// handle DOWN
					else if (seq[1] == 'B')
					{
						if (commandLen == 0) { return 0; }
						commandIndex = (commandIndex - 1 + MAX_NUM_COMMAND_HISTORY) % MAX_NUM_COMMAND_HISTORY;
					}
					clearLine(inputLength);
					write(STDOUT_FILENO, commandHistory[commandIndex], sizeof commandHistory[commandIndex]);
				}
				continue;
			}
		}
		inputBuffer[inputLength++] = c;
		write(STDOUT_FILENO, &c, 1);
	}

	tcsetattr(STDIN_FILENO, TCSAFLUSH, tsOld);
	return 0;
}