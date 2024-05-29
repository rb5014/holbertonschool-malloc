#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	void *start, *ptr;

	start = sbrk(0);
	printf("Starting break is %p\n", start);
	ptr = sbrk(1);
	printf("Break after extension is %p\n", ptr);
	*(char *)start = 'H';
	printf("Adress of our char is %p\n", start);
	printf("Value of our char is %c\n", *(char *)start);
	return (EXIT_SUCCESS);
}