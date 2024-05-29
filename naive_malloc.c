#include "malloc.h"

#define MAX_TYPE_SIZE 8

/**
 * naive_malloc - naive version of malloc:
 *	dynamically allocates memory on the heap using sbrk
 * @size: number of bytes to allocate
 * The function must return a pointer to the allocated memory
 * that is suitably aligned for any kind of variable
 * It should be able to:
 *		Allocate enough memory to store
 *			A size_t as the chunk header
 *			The size requested as parameter
 *		Allocate memory pages only
 * Return: the memory address newly allocated, or NULL on error
 *
 * Note: don't do this at home :)
 */
void *naive_malloc(size_t size)
{
	static void *start_heap;
	static size_t nb_chunks;
	size_t i, align = MAX_TYPE_SIZE;
	long page_size = sysconf(_SC_PAGESIZE);
	size_t tot_size = 0, unused_size = 0;
	void *chunk = NULL;

	if (!start_heap) /* If first call */
		start_heap = sbrk(0);

	size = (size + (align - 1)) & -align; /* Align to 8 (biggest type size)*/
	size += sizeof(size_t); /* Add header size */

	/* Loop through already used chunks to get unused chunk (last one)*/
	for (i = 0, chunk = start_heap; i < nb_chunks; i++)
		/* Go to next chunk by adding its size */
		chunk =  (char *)chunk + *(size_t *)chunk;

	/* If last chunk not current program break, then there must be unused mem */
	unused_size = (chunk < sbrk(0)) ? *(size_t *)chunk : 0;
	/* Loop adding page_size to total if not enough space */
	while (tot_size + unused_size < size + sizeof(size_t))
		tot_size += page_size;

	if (tot_size) /* We extend program break if unused isn't enough */
	{
		/* Without modifying chunk adress*/
		if (unused_size)
			sbrk(tot_size);
		/* chunk get new adress */
		else
			chunk = sbrk(tot_size);
	}

	*(size_t *)chunk = size; /* Add to chunk header the chunk size */
	/* Add to unused chunk header the unused size */
	*(size_t *)((char *)chunk + size) = tot_size + unused_size - size;

	nb_chunks++;
	return ((char *)chunk + sizeof(size_t));
}
