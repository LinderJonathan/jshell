#include "lib/global.h"
#include "lib/jshell.h"
#include "lib/util.h"
int main()
{

	char path[MAX_PATH_SIZE];
	char inputBuff[MAX_INPUT_SIZE];
	char *args[MAX_NUM_OF_ARGS];
	int builtInHandled = 0;
	int shellRunning = 1;
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
		
	while (shellRunning)
	{
		builtInHandled = 0;
		if (getcwd(path, sizeof(path)) != NULL)
		{
			fgets(inputBuff, MAX_INPUT_SIZE, stdin);
			inputBuff[strcspn(inputBuff, "\n")] = '\0';
			parseArgs(args, inputBuff);

			if (args[0] == NULL)
			{
				printf("%s > ", path);
				continue;
			}
			
			for (int i = 0; i < NUM_BUILTIN; i++)
			{
				if (strcmp(args[0],builtIns[i].name) == 0)
				{
					builtIns[i].func(args);
					builtInHandled = 1;
					break;
				}
			}
			if (builtInHandled)
			{
				printf("%s > ", path);
				continue;
			}
			else
			{
				__pid_t pid = getpid();
				__pid_t child_pid = fork();
				__pid_t child_ppid = getppid();

				if (child_pid == -1)
				{
					perror("Could not create fork");
					exit(EXIT_FAILURE);
				}
				if (pid == child_ppid)
				{
					execvp(args[0], args);
					perror("execvp");
					exit(EXIT_FAILURE);
				}
				// execute built-in commands in parent process
				wait(NULL);
			}

			getcwd(path, sizeof(path));
			printf("%s > ", path);
		}
	}
	return 0;
}