# Makefile for dprod_ed

CXXFLAGS=-pedantic -std=c++11 -O2 -march=native

build: main.o ui.o db.o
	$(CXX) $(CXXFLAGS) main.o ui.o db.o -o dprod_ed -lsqlite3 -lmenu -lform -lncurses
main.o: ui.h main.cpp

ui.o: ui.cpp ui.h db.h

db.o: db.cpp db.h

clean:
	rm -I *.o dprod_ed
