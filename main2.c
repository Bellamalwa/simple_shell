#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/**
 * struct info - Structure to hold information about the shell
 *
 * @readfd: File descriptor for standard input
 * @envp: Array of environment variables
 * @history: Array of history entries
 */
struct info
{
int readfd;
char **envp;
char **history;
};

/**
 * main - Entry point
 *
 * @argc: Number of arguments passed to the program
 * @argv: Array of strings containing the arguments
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
struct info info = 
{
.readfd = 2,
.envp = environ,
.history = NULL,
};

  /* Check the number of arguments */
if (argc == 2) {
    /* Open the file specified by the first argument */
info.readfd = open(argv[1], O_RDONLY);
if (info.readfd == -1)
{
      /* Check the error code */
if (errno == EACCES) {
        /* Permission denied */
exit(126);
} else if (errno == ENOENT) {
        /* File not found */
fprintf(stderr, "%s: 0: Can't open %s\n", argv[0], argv[1]);
exit(127);
} else {
        /* Unknown error */
exit(EXIT_FAILURE);
}
}
}

  /* Populate the environment variables */
populate_env_list(&info);

  /* Read the history file */
read_history(&info);

  /* Execute the command specified by the first argument */
hsh(&info, argv);

return (EXIT_SUCCESS);
}
