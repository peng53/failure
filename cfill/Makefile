CXXFLAGS=-pedantic -std=c++11 -O2 -march=native
CXX=clang++

build: main.o floodalg.o floodboard.o nout.o
	$(CXX) $(CXXFLAGS) main.o floodalg.o floodboard.o nout.o -lcurses -o cfill

main.o: floodalg.h floodboard.h nout.h
floodalg.o:
floodboard.o: floodalg.h
nout.o:
cfloodboard: cfloodboard.cpp floodboard.h floodalg.h
	$(CXX) $(CXXFLAGS) -c cfloodboard.cpp -o floodboard.o

clean:
	rm *.o cfill
