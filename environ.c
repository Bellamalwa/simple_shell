#include "main.h"

/**
 * printEnv - prints the current environment
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int printEnv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - gets the value of @name variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *pointer;

	while (node)
	{
		pointer = starts_with(node->str, name);
		if (pointer && *pointer)
			return (pointer);
		node = node->next;
	}
	return (NULL);
}

/**
 * setENV - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments.
 *  Return: Always 0
 */
int setENV(info_t *info)
{
	if (info->argc != 3)
	{
		printCharError("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * unsetENV - Remove an environment variable
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int unsetENV(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		printCharError("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);

	return (0);
}

/**
 * generate_env_list - generates a env linked list
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int generate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t index;

	for (index = 0; environ[index]; index++)
		add_node_end(&node, environ[index], 0);
	info->env = node;
	return (0);
}

