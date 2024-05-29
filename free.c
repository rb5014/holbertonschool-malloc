#include "malloc.h"

/**
 * _free - frees a memory space
 * @ptr: pointer to the memory space to be freed
*/
void _free(void *ptr)
{
	size_t size, prev_size;

	if (!ptr)
		return;

	/* Get chunk size stored in the second part of the chunk's header*/
	size = *(size_t *)((void *)ptr - SIZE_T);
	/* Get prev chunk size stored in the first part of the chunk's header*/
	prev_size = *(size_t *)((void *)ptr - 2 * SIZE_T);

	/* If previous one is already freed (prev_size != 0), merge both together */
	size += prev_size;

	/**
	 * Update first part of next chunk's header
	 * to indicate that current chunk is free
	*/
	*(size_t *)((void *)ptr + size) = size;

}
