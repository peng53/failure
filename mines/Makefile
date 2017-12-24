CC=gcc
CFLAGS=-pedantic -std=c99 -Wall

build: main.o
	$(CC) $(CFLAGS) main.o -o mines -lm -lncurses
buildpd: main.o
	g++ main.o -o mines -lm -lcurses -D UPDCURSES
onlynzero: main.o
	$(CC) $(CFLAGS) -c main.c -D ONLYNZERO
default main.o: main.c
	$(CC) $(CFLAGS) -c main.c
clean:
	rm -I *.o mines
