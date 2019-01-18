# Makefile for chunkreader

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects
objects=$(OBJ)/chr_main.o $(OBJ)/chread.o $(OBJ)/stream_test.o $(OBJ)/ireader_extf.o $(OBJ)/ireaderfactory.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o ../bin/chr

$(OBJ)/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chr_main.o: main.cpp ireaderfactory.h ireader_extf.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm $(objects) ../bin/chr

test: ../bin/chr
	../bin/chr

