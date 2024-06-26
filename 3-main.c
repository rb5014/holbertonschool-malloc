#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *naive_malloc_header(size_t size)
{
	void *chunk;

	size += sizeof(size_t);
	chunk = sbrk(size);
	*(size_t *)chunk = size;

	return (chunk + sizeof(size_t));
}

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	char *str;
	int i;

	printf("Starting break is %p\n", sbrk(0));

	for (i = 0; i < 10; i++)
	{
		void *chunk;

		str = naive_malloc_header(10);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk starting at address %p, ", (void *)chunk);
		printf("chunk size is %lu\n", *((size_t *)chunk));
	}

	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}
