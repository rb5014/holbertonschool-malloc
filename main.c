#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "malloc.h"

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

	for (i = 0; i < 20; i++)
	{
		void *chunk;

		str = _malloc(1008);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - 2 * sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("prev size: %lu, ", *(size_t *)chunk);
		printf("size: %lu, ", *((size_t *)((char *)chunk + sizeof(size_t))));
		printf("break: %p\n", sbrk(0));
		if (i == 2)
			_free(str);
	}

	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}