# Makefile for srstrgen

CXXFLAGS=-pedantic -O2 -march=native -Wall -Werror
OBJ=../objects
objects=$(OBJ)/b_inc.o $(OBJ)/st.o $(OBJ)/srstrgen_main.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o ../bin/srstrgen

$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/srstrgen_main.o: main.cpp st.h b_inc.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@

lib_exo: $(OBJ)/exo.o $(OBJ)/b_inc.o $(OBJ)/st.o
	$(CXX) $(CXXFLAGS) -shared $< -o ../bin/lib_exo.so

test:
	../bin/srstrgen -c 3 -b 001000 abc - -s 0,4,1,2 " cool cat"
clean:
	rm -I $(objects) ../bin/srstrgen ../bin/lib_exo.so
