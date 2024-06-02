#include "malloc.h"

/**
 * _free - frees a memory space
 * @ptr: pointer to the memory space to be freed
*/
void _free(void *ptr)
{
	void *chunk;
	size_t size, prev_size, tot_size;

	if (!ptr)
		return;

	chunk = (void *)((char *)ptr - 2 * SIZE_T);

	/* Get chunk size stored in the second part of the chunk's header*/
	size = *(size_t *)((char *)chunk + SIZE_T);
	tot_size = size;
	/* Get prev chunk size stored in the first part of the chunk's header*/
	prev_size = *(size_t *)chunk;
	/* If previous one is already freed (prev_size != 0), merge both together */
	if (prev_size)
	{
		void *prev_chunk = (void *)((char *)chunk - prev_size);

		tot_size += prev_size;
		/* Update previous chunk size */
		*((char *)prev_chunk + SIZE_T) = tot_size;
	}

	/**
	 * Update first part of next chunk's header
	 * to indicate that current chunk is free
	*/
	*(size_t *)((char *)chunk + size) = tot_size;
}
