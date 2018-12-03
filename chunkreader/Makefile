# Makefile for chunkreader

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects

build: $(OBJ)/chr_main.o $(OBJ)/chread.o
	$(CXX) $(CXXFLAGS) $(OBJ)/chr_main.o $(OBJ)/chread.o -o ../bin/chr

stream_test: $(OBJ)/stream_test.o
	$(CXX) $(CXXFLAGS) $< -o ../bin/stream_test

$(OBJ)/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chr_main.o: main.cpp chread.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

clean:
	rm $(OBJ)/main.o $(OBJ)/chread.o ../bin/chr

test1: ../bin/stream_test
	../bin/stream_test

test: ../bin/chr
	../bin/chr

