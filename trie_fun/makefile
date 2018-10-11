CXXFLAGS=-pedantic -O2 -march=native -std=c++11
CXX=clang++

cnt_word.o: cnt_word.cpp wordbank.h
	$(CXX) -c $(CXXFLAGS) cnt_word.cpp

wordbank.o: wordbank.cpp wordbank.h
	$(CXX) -c $(CXXFLAGS) wordbank.cpp

st.o: ../srstrgen_pm/st.cpp ../srstrgen_pm/st.h
	$(CXX) -c $(CXXFLAGS) ../srstrgen_pm/st.cpp -o ../srstrgen_pm/st.o -std=c++11

main.o: main.cpp ../srstrgen_pm/st.h
	$(CXX) -c $(CXXFLAGS) main.cpp

cnt_word: cnt_word.o wordbank.o
	$(CXX) $(CXXFLAGS) cnt_word.o wordbank.o

main: wordbank.o ../srstrgen_pm/st.o main.o
	$(CXX) $(CXXFLAGS) main.o wordbank.o ../srstrgen_pm/st.o -o main

clean:
	del *.o
	del *.exe
