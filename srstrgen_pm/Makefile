CFLAGS=-pedantic -std=c++11
CC=g++

build: main.o ste.o b_inc.o
	$(CC) $(CFLAGS) main.o ste.o b_inc.o -o srstrgen
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
ste.o: ste.cpp ste.h
	$(CC) $(CFLAGS) -c ste.cpp
b_inc.o: b_inc.cpp b_inc.h
	$(CC) $(CFLAGS) -c b_inc.cpp

st.o: st.cpp st.h
	$(CC) $(CFLAGS) -c st.cpp -o ste.o


compile:
	g++ -pedantic -std=c++11 -c st.cpp
	g++ -pedantic -std=c++11 -c ste.cpp
	g++ -pedantic -std=c++11 -c b_inc.cpp
	g++ -pedantic -std=c++11 -c main.cpp
test:
	./srstrgen -c 3 -b 001000 abc - BOB CAS EGD -s 0,4,1,2 " cool cat" " " 2,4,0
clean:
	rm -I *.o *.gch srstrgen
