#include "shell.h"

/**
 * clear_info - initializes @info struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes @info struct with @argVector
 * @info: struct address
 * @argVector: argument vector
 */
void set_info(info_t *info, char **argVector)
{
	int i = 0;

	info->fname = argVector[0];
	if (info->arg)
	{
		info->argv = strtoken(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - frees @info struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	freeStrings(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		freeStrings(info->environ);
			info->environ = NULL;
		freePointer((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}

