CFLAGS=-pedantic -std=c++11 -O2 -march=native
CC=g++

build: main.o floodalg.o floodboard.o
	$(CC) $(CFLAGS) main.o floodalg.o floodboard.o -o cfill
main.o: main.cpp floodalg.h floodboard.h
	$(CC) $(CFLAGS) -c main.cpp
floodalg.o: floodalg.cpp floodalg.h
	$(CC) $(CFLAGS) -c floodalg.cpp
floodboard.o: floodboard.cpp floodboard.h floodalg.h
	$(CC) $(CFLAGS) -c floodboard.cpp
cfloodboard: cfloodboard.cpp floodboard.h floodalg.h
	$(CC) $(CFLAGS) -c cfloodboard.cpp -o floodboard.o
clean:
	rm *.o cfill
