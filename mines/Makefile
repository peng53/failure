FLAGS=-pedantic -std=c99

build: main.o
	gcc $(FLAGS) main.o -o mines -lm -lncurses
main.o: main.c
	gcc $(FLAGS) -c main.c
clean:
	rm -I *.o mines
