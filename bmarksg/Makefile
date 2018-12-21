# Makefile for bmarksg

CXXFLAGS=-pedantic -O2 -march=native -Wall -Werror
OBJ=../objects

build: $(OBJ)/bmarksg.o $(OBJ)/bmarksg_main.o
	$(CXX) $(CXXFLAGS) $(OBJ)/bmarksg_main.o $(OBJ)/bmarksg.o -o ../bin/bmarksg -lsqlite3

$(OBJ)/bmarksg_main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm $(OBJ)/bmarksg_main.o $(OBJ)/bmarksg.o ../bin/bmarksg
