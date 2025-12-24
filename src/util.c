#include "../lib/global.h"
#include "../lib/util.h"

builtIn builtIns[] = 
{
	{
		"jcd",
		builtInJcd
	}
};

void parseArgs(char* args[], char inputBuff[])
{
	__uint8_t i = 0;
	__uint8_t k = 0;
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


void builtInJcd(char *args[])
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "cd: no path provided\n");
		return;
	}
	if (chdir(args[1]) == -1)
	{
		perror("cd");
	}
}