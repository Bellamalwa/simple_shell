#include "shell.h"

/**
 * _memset - fills memory with @b
 * @s: the pointer to the memory area
 * @b: the byte to fill *s with
 * @n: the amount of bytes to be filled
 * Return: (s) a pointer to the memory area s
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return (s);
}

/**
 * freeStrings - frees up strings within the linked list
 * @pointer_string: string of strings
 */
void freeStrings(char **pointer_string)
{
	char **string = pointer_string;

	if (!pointer_string)
		return;
	while (*pointer_string)
		free(*pointer_string++);
	free(string);
}

/**
 * _realloc - reallocates a block of memory
 * @ptr: pointer to previously allocated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 *
 * Return: pointer to newly allocated memory block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (p);
}

