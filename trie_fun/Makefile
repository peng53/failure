# Makefile for trie_fun

CXXFLAGS=-pedantic -O2 -march=native -std=c++11
CSC=csc
OBJ=../objects
objects=$(OBJ)/trie_fun_main.o $(OBJ)/cnt_word.o $(OBJ)/wordbank.o $(OBJ)/st.o

cnt_word: $(OBJ)/cnt_word.o $(OBJ)/wordbank.o
	$(CXX) $(CXXFLAGS) $(OBJ)/cnt_word.o $(OBJ)/wordbank.o -o ../bin/cnt_word

$(OBJ)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(OBJ)/st.o: ../srstrgen_pm/st.cpp ../srstrgen_pm/st.h
	$(CXX) -c $(CXXFLAGS) ../srstrgen_pm/st.cpp -o $@

$(OBJ)/trie_fun_main.o: main.cpp ../srstrgen_pm/st.h wordbank.h
	$(CXX) -c $(CXXFLAGS) main.cpp -o $@


main: $(OBJ)/wordbank.o $(OBJ)/st.o $(OBJ)/trie_fun_main.o
	$(CXX) $(CXXFLAGS) $(OBJ)/trie_fun_main.o $(OBJ)/wordbank.o $(OBJ)/st.o -o ../bin/trie_fun

clean:
	rm $(objects) ../bin/cnt_word ../bin/trie_fun_main
