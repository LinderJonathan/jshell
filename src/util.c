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

const int NUM_BUILTIN = sizeof(builtIns) / sizeof(builtIn);

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

}

int isBuiltIn(const char *cmd)
{
	for (int i = 0; i < NUM_BUILTIN; i++)
	{
		if (strcmp(cmd, builtIns[i].name) == 0)
		{
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