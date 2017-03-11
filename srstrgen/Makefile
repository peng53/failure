build:
	g++ -pedantic -std=c++11 main.cpp -o srstrgen
compile:
	g++ -pedantic -std=c++11 -c st.cpp
	g++ -pedantic -std=c++11 -c b_inc.cpp
	g++ -pedantic -std=c++11 -c main.cpp
lib_exo:
	g++ -pedantic -std=c++11 -shared "exo.cpp" -o lib_exo.so
test:
	./srstrgen -c 3 -b 001000 abc - -s 0,4,1,2 " cool cat"
clean:
	rm -I *.o *.gch srstrgen lib_exo.so
