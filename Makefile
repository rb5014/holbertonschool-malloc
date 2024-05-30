all: clean
	gcc -g -Wall -Werror -Wextra -pedantic main.c malloc.c free.c

clean:
	rm -f ./a.out