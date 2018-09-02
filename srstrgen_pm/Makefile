CFLAGS=-pedantic -std=c++11 -O2 -march=native
CC=clg++

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
	$(CC) -pedantic -std=c++11 -c st.cpp
	$(CC) -pedantic -std=c++11 -c ste.cpp
	$(CC) -pedantic -std=c++11 -c b_inc.cpp
	$(CC) -pedantic -std=c++11 -c main.cpp
test:
	./srstrgen -c 3 -b 001000 abc - BOB CAS EGD -s 0,4,1,2 " cool cat" " " 2,4,0
clean:
	rm -I *.o *.gch srstrgen

parted.cs:
	csc /t:library /out:parted.dll parted.cs

main.cs: parted.cs
	csc /reference:parted.dll /t:exe /out:main.exe