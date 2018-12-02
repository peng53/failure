# Makefile for dprod_ed

CXXFLAGS=-pedantic -std=c++11 -O2 -march=native
OBJ=../objects/dprod_ed
objects=$(OBJ)/main.o $(OBJ)/ui.o $(OBJ)/db.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o ../bin/dprod_ed -lsqlite3 -lmenu -lform -lncurses

$(OBJ)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -I ../bin/dprod_ed $(objects)
