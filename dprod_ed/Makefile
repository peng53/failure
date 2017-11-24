FLAGS=-pedantic -std=c++11

build: ui.o db.o
	g++ $(FLAGS) main.cpp ui.o db.o -o dprod_ed -lsqlite3 -lncurses -lform
ui.o: ui.cpp ui.h db.o
	g++ $(FLAGS) -c ui.cpp
db.o: db.cpp db.h
	g++ $(FLAGS) -c db.cpp
clean:
	rm -I *.o *.gch dprod_ed
