all: main.o
	gcc main.o -o ex1
clean:
	rm -rf *.o ex1
main.o: main.c
	gcc -std=c99 -Wall -Wextra -Werror -pedantic-errors -c main.c
