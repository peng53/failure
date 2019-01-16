# Makefile for chunkreader

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects

build: $(OBJ)/chr_main.o $(OBJ)/chread.o $(OBJ)/stream_test.o
	$(CXX) $(CXXFLAGS) $(OBJ)/chr_main.o $(OBJ)/chread.o $(OBJ)/stream_test.o -o ../bin/chr

$(OBJ)/stream_test_main.o: stream_test_main.cpp
	$(CXX) -c $(CXXFLAGS) stream_test_main.cpp -o $@

stream_test_main: $(OBJ)/stream_test_main.o $(OBJ)/stream_test.o
	$(CXX) $(CXXFLAGS) $(OBJ)/stream_test_main.o $(OBJ)/stream_test.o -o ../bin/stream_test_main

$(OBJ)/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chr_main.o: main.cpp chread.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

clean:
	rm $(OBJ)/main.o $(OBJ)/chread.o ../bin/chr $(OBJ)/stream_test.o $(OBJ)/stream_test_main.o ../bin/stream_test_main

test1: ../bin/stream_test_main
	../bin/stream_test_main

test: ../bin/chr
	../bin/chr

