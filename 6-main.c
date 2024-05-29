#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "malloc.h"

#define MAX_TYPE_SIZE 8
void *naive_malloc_align(size_t size)
{
	static void *start_heap;
	static int nb_chunks;
	int i, align = MAX_TYPE_SIZE;
	long page_size = sysconf(_SC_PAGESIZE);
	size_t tot_size = 0, unused_size = 0;
	void *chunk, *tmp;

	/* Formula to align to 8 (biggest type size)*/
	size = (size + (align - 1)) & -align;
	size += sizeof(size_t);
	if (!start_heap)
	{
		/* Get the total size to allocate that is a multiple of page size */
		while (tot_size < size + sizeof(size_t))
			tot_size += page_size;

		start_heap = sbrk(tot_size);
		*(size_t *)start_heap = size;
		*(size_t *)(start_heap + size) = tot_size - size;
		chunk = start_heap;
	}
	else
	{
		for (i = 0, chunk = start_heap; i < nb_chunks; i++)
			chunk += *(size_t *)chunk; /* Go to next chunk by adding its size */
		unused_size = (chunk < sbrk(0)) ? *(size_t *)chunk : 0;
		tot_size = unused_size;
		while (tot_size < size + sizeof(size_t))
			tot_size += page_size;
		if (tot_size > unused_size)
			tmp = sbrk(tot_size);
		*(size_t *)chunk = size;
		if (tot_size > size)
			*(size_t *)(chunk + size) = tot_size - size;
	}
	nb_chunks++;

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

		str = naive_malloc(10);
		strcpy(str, "Holberton");
		str[9] = '\0';
		printf("%p: %s, ", (void *)str, str);
		chunk = (void *)(str - sizeof(size_t));
		printf("chunk addr: %p, ", (void *)chunk);
		printf("size: %lu, ", *((size_t *)chunk));
		printf("break: %p\n", sbrk(0));
	}

	printf("Final break is %p\n", sbrk(0));
	return (EXIT_SUCCESS);
}
