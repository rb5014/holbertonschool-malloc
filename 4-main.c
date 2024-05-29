#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>


void *naive_malloc_page(size_t size)
{
	static void *start_heap;
	static int nb_chunks;
	int i;
	long page_size = sysconf(_SC_PAGESIZE);
	size_t tot_size = 0, unused_size;
	void *chunk, *tmp;

	size += sizeof(size_t);
	if (!start_heap)
	{
		/* Get the total size to allocate that is a multiple of page size */
		while (/* tot size smaller than requested */
			   (size > tot_size) ||
			   /* OR tot_size big enough, but not for unused space */
			   ((tot_size > size) && (tot_size < size + sizeof(size_t))))
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
		unused_size = *(size_t *)chunk;
		tot_size = unused_size;
		while ((size > tot_size) ||
			   ((tot_size > size) && (tot_size < size + sizeof(size_t))))
			tot_size += page_size;
		if (tot_size > unused_size)
			tmp = sbrk(tot_size);
		*(size_t *)chunk = size;
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

		str = naive_malloc_page(10);
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