FLAGS=-pedantic -std=c++11 -O2 -march=native

build: main.o ui.o db.o
	g++ $(FLAGS) main.o ui.o db.o -o dprod_ed -lsqlite3 -lmenu -lform -lncurses
main.o: ui.h db.h main.cpp
	g++ $(FLAGS) -c main.cpp
ui.o: ui.cpp ui.h db.o
	g++ $(FLAGS) -c ui.cpp
db.o: db.cpp db.h
	g++ $(FLAGS) -c db.cpp
clean:
	rm -I *.o *.gch dprod_ed
