build:
	g++ -pedantic -std=c++11 main.cpp -o srstrgen
compile:
	g++ -pedantic -std=c++11 -c st.cpp
	g++ -pedantic -std=c++11 -c ste.cpp
	g++ -pedantic -std=c++11 -c b_inc.cpp
	g++ -pedantic -std=c++11 -c main.cpp
test:
	./srstrgen -c 3 -b 001000 abc - BOB CAS EGD -s 0,4,1,2 " cool cat" " " 2,4,0
clean:
	rm -I *.o *.gch srstrgen
