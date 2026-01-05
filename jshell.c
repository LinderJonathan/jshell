#include "lib/global.h"
#include "lib/jshell.h"
#include "lib/util.h"

static struct termios tsOrig;
char *commandHistory[MAX_NUM_COMMAND_HISTORY];
int commandHead = 0;
int commandEnd = 0;
int commandIndex = 0;
void handleSignal(int signal)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tsOrig);
	printf("signal triggered\n");
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "signal handling\n", 15);
		write(STDOUT_FILENO, "\n", 0);
		exit(EXIT_SUCCESS);
	}

	// TODO: reset termios configuration
}

int main()
{
	struct termios tsNew;

	// save original settings 
	tcgetattr(STDIN_FILENO, &tsOrig);
	tsNew = tsOrig;
	tsNew.c_lflag &= ~(ICANON | ECHO);
	tsNew.c_cc[VMIN] = 1;
	tsNew.c_cc[VTIME] = 0;

	struct sigaction sa = {.sa_handler = handleSignal};
	int sigret = sigaction(SIGINT, &sa, NULL);

	char path[MAX_PATH_SIZE];
	char inputBuff[MAX_INPUT_SIZE];
	char *args[MAX_NUM_OF_ARGS];
	int fetchedCommandHistory = 0;
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
	printPath(path, sizeof path);
	while (shellRunning)
	{
		
		builtInHandled = 0;
		if (getcwd(path, sizeof(path)) != NULL)
		{
			readInput(&tsNew, &tsOrig, inputBuff, sizeof inputBuff);

			inputBuff[strcspn(inputBuff, "\n")] = '\0';
			parseArgs(args, inputBuff);

			if (args[0] == NULL)
			{
				printPath(path, sizeof path);
				continue;
			}

			// TODO: fill command history with input in buffer each iteration
			commandHistory[0] = inputBuff;

			printf("%s", commandHistory[0]);
			int builtInHandled = runBuiltIn(args);
			if (builtInHandled == BUILT_IN_NOT_HANDLED)
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
			printPath(path, sizeof path);
		}
	}

	// restore terminal settings
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tsOrig);
	return 0;
}

