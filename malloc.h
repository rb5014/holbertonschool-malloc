#ifndef MALLOC_H
#define MALLOC_H
#include <stdlib.h>
#include <unistd.h>
#define SIZE_T sizeof(size_t)
#define MAX_TYPE_SIZE 8
#define chunk_size(chunk) (*(size_t *)((char *)chunk + SIZE_T))
#define prev_chunk_size(chunk) (*(size_t *)chunk)
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
#endif /* MALLOC_H */
