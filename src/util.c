#include "../lib/global.h"
#include "../lib/util.h"

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

void handleSignal(int signal)
{
	if (signal == SIGTERM)
	{
		write(STDOUT_FILENO, "signal handling\n", 15);
		write(STDOUT_FILENO, "\n", 0);
		exit(EXIT_SUCCESS);
	}

	// TODO: reset termios configuration
}

int readInput(struct termios *tsNew, struct termios *tsOld, char *inputBuffer, size_t bufferSize)
{
	memset(inputBuffer, 0, bufferSize);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, tsNew);
	int i = 0;
	char c;
	while (i < bufferSize)
	{
		// if byte read and written to buffer
		if (read(STDIN_FILENO, &c, 1) == 1)
		{
			// handle enter
			if (c == '\n')
			{
				write(STDOUT_FILENO, "\n", 1);
				tcsetattr(STDIN_FILENO, TCSAFLUSH, tsOld);
				return 1;
			}

			// handle backspace
			if (c == 0x7f || c == '\b')
			{
				// TODO: if i > 0, set value at index i-1 to NULL. decrement i
				if (i > 0)
				{
					i--;
					inputBuffer[i] = '\0';
					write(STDOUT_FILENO, "\b \b", 3);
				}
				continue;
			}
			// handle arrow up or down
			if (c == '\x1b')
			{
				char seq[2];
				read(STDIN_FILENO, &seq[0], 1);
				read(STDIN_FILENO, &seq[1], 1);
				if (seq[0] == '[')
				{
					// handle UP
					if (seq[1] == 'A')
					{
						printf("up arrow pressed\n");
						// TODO: fetch from command history 

						// TODO: write bytes to command line
					}
					// handle DOWN
					else if (seq[1] == 'B')
					{
						printf("down arrow pressed\n");
						// TODO: fetch from command history array

						// TODO: write bytes to command line
					}
				}
			}
			inputBuffer[i] = c;
			write(STDOUT_FILENO, &inputBuffer[i], 1);
		}
		i++;
	}

	//read(STDIN_FILENO, &c, 1);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, tsOld);
	return 0;
}