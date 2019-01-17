# Makefile for chunkreader

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects

build: $(OBJ)/chr_main.o $(OBJ)/chread.o $(OBJ)/stream_test.o $(OBJ)/ireader_extf.o
	$(CXX) $(CXXFLAGS) $(OBJ)/chr_main.o $(OBJ)/chread.o $(OBJ)/stream_test.o $(OBJ)/ireader_extf.o -o ../bin/chr

$(OBJ)/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chr_main.o: main.cpp ireader.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

clean:
	rm $(OBJ)/chread.o $(OBJ)/stream_test.o $(OBJ)/chr_main.o $(OBJ)/ireader_extf.o ../bin/chr

test: ../bin/chr
	../bin/chr

