#include "malloc.h"
#include <stdio.h>

#define ALIGN(size) ((size + MAX_TYPE_SIZE) & -MAX_TYPE_SIZE)
static void *start_heap;
static size_t nb_chunks;

/**
 * _malloc - dynamically allocates memory on the heap using sbrk
 * @size: number of bytes to allocate for the user
 * Return: the memory address newly allocated, or NULL on error
 */
void *_malloc(size_t size)
{
	size_t h_size = 2 * SIZE_T;
	long page_size = sysconf(_SC_PAGESIZE);
	size_t tot_size = 0, unused_size = 0;
	void *chunk = NULL;

	if (!start_heap) /* If first call */
		start_heap = sbrk(0);
	size = ALIGN(size); /* Align to 8 (biggest type size)*/
	size += h_size;		/* Add header size */
	/* Loop through already used chunks to get unused chunk (last one)*/
	for (chunk = start_heap;
		 chunk < sbrk(0) && (void *)((char *)chunk + chunk_size(chunk)) < sbrk(0);
		 chunk = (char *)chunk + chunk_size(chunk))
	{
		if (prev_chunk_size(chunk) >= size)
		{
			chunk = (char *)chunk - prev_chunk_size(chunk);
			break;
		}
	}
	/* If last chunk not current program break, then there must be unused mem */
	unused_size = (chunk < sbrk(0)) ? chunk_size(chunk) : 0;
	/* Loop adding page_size to total if not enough space */
	while (tot_size + unused_size < size + h_size)
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
	/* Add to chunk header the chunk size */
	*(size_t *)((char *)chunk + SIZE_T) = size;
	/* Add to unused chunk header the unused size */
	*(size_t *)((char *)chunk + size + SIZE_T) = tot_size + unused_size - size;
	nb_chunks++;
	return ((char *)chunk + h_size);
}
