#include "main.h"

/**
 * input_buffer - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buffer(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* TODO: remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line without the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf;
	static size_t chain_pos, chain_count, chain_buffer;
	ssize_t result = 0;
	char **buf_p = &(info->arg), *chain_pointer;

	_putchar(BUF_FLUSH);
	result = input_buffer(info, &buf, &chain_buffer);
	if (result == -1) /* NOTE: EOF */
		return (-1);
	if (chain_buffer)
	{
		chain_count = chain_pos;
		chain_pointer = buf + chain_pos;

		check_chain(info, buf, &chain_count, chain_pos, chain_buffer);
		while (chain_count < chain_buffer)
		{
			if (is_chain_char(info, buf, &chain_count))
				break;
			chain_count++;
		}

		chain_pos = chain_count + 1;
		if (chain_pos >= chain_buffer)
		{
			chain_pos = chain_buffer = 0;
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = chain_pointer;
		return (_strlen(chain_pointer));
	}

	*buf_p = buf; /* NOTE: if not a chain, return buffer from _getline() */
	return (result);
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: string
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t buffer_pos, buffer_len;
	size_t k;
	ssize_t result = 0, string = 0;
	char *pointer = NULL, *new_pointer = NULL, *c;

	pointer = *ptr;
	if (pointer && length)
		string = *length;
	if (buffer_pos == buffer_len)
		buffer_pos = buffer_len = 0;

	result = read_buf(info, buf, &buffer_len);
	if (result == -1 || (result == 0 && buffer_len == 0))
		return (-1);

	c = _strchr(buf + buffer_pos, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : buffer_len;
	new_pointer = _realloc(pointer, string, string ? string + k : k + 1);
	if (!new_pointer)
		return (pointer ? free(pointer), -1 : -1);

	if (string)
		_strncat(new_pointer, buf + buffer_pos, k - buffer_pos);
	else
		_strncpy(new_pointer, buf + buffer_pos, k - buffer_pos + 1);

	string += k - buffer_pos;
	buffer_pos = k;
	pointer = new_pointer;

	if (length)
		*length = string;
	*ptr = pointer;
	return (string);
}

/**
 * sigintHandler - overrides Ctrl+c
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

