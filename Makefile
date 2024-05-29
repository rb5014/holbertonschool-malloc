all: clean
	gcc -g 6-main.c naive_malloc.c 

clean:
	rm -f ./a.out