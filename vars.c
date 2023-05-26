#include "shell.h"

/**
 * is_chain_char - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain_char(info_t *info, char *buf, size_t *p)
{
	size_t index = *p;

	if (buf[index] == '|' && buf[index + 1] == '|')
	{
		buf[index] = 0;
		index++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[index] == '&' && buf[index + 1] == '&')
	{
		buf[index] = 0;
		index++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[index] == ';') /* found end of this command */
	{
		buf[index] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = index;
	return (1);
}

/**
 * check_chain - checks if we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t index = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			index = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			index = len;
		}
	}

	*p = index;
}

/**
 * replace_alias - replaces an alias
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
	int index;
	list_t *node;
	char *pointer;

	for (index = 0; index < 10; index++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		pointer = _strchr(node->str, '=');
		if (!pointer)
			return (0);
		pointer = _strdup(pointer + 1);
		if (!pointer)
			return (0);
		info->argv[0] = pointer;
	}
	return (1);
}

/**
 * replace_vars - replace variable in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
	int index = 0;
	list_t *node;

	for (index = 0; info->argv[index]; index++)
	{
		if (info->argv[index][0] != '$' || !info->argv[index][1])
			continue;

		if (!_strcmp(info->argv[index], "$?"))
		{
			replace_string(&(info->argv[index]),
					_strdup(convert_number(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[index], "$$"))
		{
			replace_string(&(info->argv[index]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->env, &info->argv[index][1], '=');
		if (node)
		{
			replace_string(&(info->argv[index]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info->argv[index], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces a string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

