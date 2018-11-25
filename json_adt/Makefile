# Makefile for json_adt

CXXFLAGS=-pedantic -O2 -march=native
OBJ=../objects

build: $(OBJ)/jso.o $(OBJ)/jso_f.o main.o
	$(CXX) $(CXXFLAGS) $(OBJ)/jso_f.o $(OBJ)/jso.o main.o -o jso_f

$(OBJ)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm jso_f $(OBJ)/jso.o $(OBJ)/jso_f.o main.o
