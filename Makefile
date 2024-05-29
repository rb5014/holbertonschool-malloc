all: clean
	gcc -g main.c malloc.c

clean:
	rm -f ./a.out