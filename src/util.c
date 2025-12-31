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

	// TODO: handle 'arrow-up'
	// TODO: handle 'arrow-down'
}