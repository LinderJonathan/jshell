#include"jshell.h"

int main()
{
	printf(
		"                __                             __       		\n"
		" _      _____  / /________  ____ ___  ___     / /_____  		\n"
		"| | /| / / _ \\/ / ___/ __ \\/ __ `__ \\/ _ \\   / __/ __ \\ 	\n"
		"| |/ |/ /  __/ / /__/ /_/ / / / / / /  __/  / /_/ /_/ / 		\n"
		"|__/|__/\\___/_/\\___/\\____/_/_/_/_/_/\\___/   \\__/\\____/   \n"
		"      / /    / ___// / / / ____/ /   / /                		\n"
		" __  / /_____\\__ \\/ /_/ / __/ / /   / /                 		\n"
		"/ /_/ /_____/__/ / __  / /___/ /___/ /___               		\n"
		"\\____/     /____/_/ /_/_____/_____/_____/               		\n"
		"\n"
		"\n"
	);

	bool shellRunning = true;
	while (shellRunning)
	{
		char path[MAX_PATH_SIZE];
		char inputBuff[MAX_INPUT_SIZE];
		if (getcwd(path, sizeof(path)) != NULL)
		{
			printf("%s > ", path);
			scanf("%s", inputBuff);
			
			// TODO: read inputBuff into a list of arguments
			char *args[] = {"ls", "-l", NULL };

			if (strcmp(inputBuff, "exit") == 0)
			{	
				// TODO: read argument array into execvp
				printf("executing ls \n");
				execvp(args[0], args);
			}
		}

	}
	return 0;
}
