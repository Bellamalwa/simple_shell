#include "shell.h"

/**
 * main - entry point
 * @argCounter: argument counter
 * @argVector: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argCounter, char **argVector)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	if (argCounter == 2)
	{
		fd = open(argVector[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				printCharError(argVector[0]);
				printCharError(": 0: Can't open ");
				printCharError(argVector[1]);
				printStringError('\n');
				printStringError(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}
	generate_env_list(info);
	read_history(info);
	loopTrigger(info, argVector);
	return (EXIT_SUCCESS);
}
