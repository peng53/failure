# Makefile for chunkreader

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects

build: main.o $(OBJ)/chread.o
	$(CXX) $(CXXFLAGS) main.o $(OBJ)/chread.o -o main

$(OBJ)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm main.o $(OBJ)/chread.o
