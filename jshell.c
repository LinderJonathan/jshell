#include "global.h"
#include"jshell.h"

int main()
{

	char path[MAX_PATH_SIZE];
	char inputBuff[MAX_INPUT_SIZE];
	char* args[MAX_NUM_OF_ARGS];
	
	printf(
		ANSI_COLOR_BLUE 
		"                __                             __       		\n" 
		" _      _____  / /________  ____ ___  ___     / /_____  		\n"
		"| | /| / / _ \\/ / ___/ __ \\/ __ `__ \\/ _ \\   / __/ __ \\ 	\n"
		"| |/ |/ /  __/ / /__/ /_/ / / / / / /  __/  / /_/ /_/ / 		\n"
		"|__/|__/\\___/_/\\___/\\____/_/_/_/_/_/\\___/   \\__/\\____/   \n"
		ANSI_COLOR_RESET 
		ANSI_COLOR_VIOLET 
		"      / /    / ___// / / / ____/ /   / /                		\n"
		" __  / /_____\\__ \\/ /_/ / __/ / /   / /                 		\n"
		"/ /_/ /_____/__/ / __  / /___/ /___/ /___               		\n"
		"\\____/     /____/_/ /_/_____/_____/_____/               		\n"
		
		ANSI_COLOR_RESET
		"\n"
		"\n"
	);

	if (getcwd(path, sizeof(path)) != NULL)
	{
		printf("%s > ", path);
	}
	
	bool shellRunning = true;
	while (shellRunning)
	{

		if (getcwd(path, sizeof(path)) != NULL)
		{
			fgets(inputBuff, MAX_INPUT_SIZE, stdin);
			inputBuff[strcspn(inputBuff, "\n")] = '\0'; // remove newline

			parseArgs(args, inputBuff);

			if (args[0])
			{

				if (strcmp(args[0], "exit") == 0)
				{
					return 0;
				}

				__pid_t pid = getpid();
				__pid_t child_pid = fork();
				__pid_t child_ppid = getppid();

				if (child_pid == -1)
				{
					perror("Could not create fork");
					exit(1);
				}
				if (pid == child_ppid)
				{
					execvp(args[0], args);
					exit(0);
				}
				wait();
				// TODO: add "unknown command"

				// TODO: add "type --help ..."
			}
			printf("%s > ", path);
		}
	}
	return 0;
}

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