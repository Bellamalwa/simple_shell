#include "main.h"

/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		writeStringFD(node->str, fd);
		writeCharFD('\n', fd);
	}
	writeCharFD(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(info_t *info)
{
	int index, last = 0, linecount = 0;
	ssize_t fd, readLen, fileSize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fileSize = st.st_size;
	if (fileSize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fileSize + 1));
	if (!buf)
		return (0);
	readLen = read(fd, buf, fileSize);
	buf[fileSize] = 0;
	if (readLen <= 0)
		return (free(buf), 0);
	close(fd);
	for (index = 0; index < fileSize; index++)
		if (buf[index] == '\n')
		{
			buf[index] = 0;
			build_history_list(info, buf + last, linecount++);
			last = index + 1;
		}
	if (last != index)
		build_history_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	arrange_history(info);
	return (info->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments.
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * arrange_history - re-arrange the history linked list after changes
 * @info: Structure containing potential arguments.
 *
 * Return: the new histcount
 */
int arrange_history(info_t *info)
{
	list_t *node = info->history;
	int index = 0;

	while (node)
	{
		node->num = index++;
		node = node->next;
	}
	return (info->histcount = index);
}

