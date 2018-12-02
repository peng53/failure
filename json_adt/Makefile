# Makefile for json_adt

CXXFLAGS=-pedantic -O2 -march=native -Wall -Werror
OBJ=../objects

build: $(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/json_adt_main.o $(OBJ)/chread.o
	$(CXX) $(CXXFLAGS) $(OBJ)/jso_f.o $(OBJ)/jso.o $(OBJ)/chread.o $(OBJ)/json_adt_main.o -o ../bin/jso_f


debug: CXXFLAGS += -DDEBUG -g
debug: build

$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chread.o: ../chunkreader/chread.cpp ../chunkreader/chread.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/json_adt_main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm ../bin/jso_f $(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/json_adt_main.o $(OBJ)/chread.o

test: ../bin/jso_f
	../bin/jso_f ../in_out/$(in)
