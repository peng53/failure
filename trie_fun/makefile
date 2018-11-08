CXXFLAGS=-pedantic -O2 -march=native

cnt_word: cnt_word.o wordbank.o
	$(CXX) $(CXXFLAGS) cnt_word.o wordbank.o

cnt_word.o: wordbank.h

wordbank.o:

main: wordbank.o ../srstrgen_pm/st.o main.o
	$(CXX) $(CXXFLAGS) main.o wordbank.o ../srstrgen_pm/st.o -o main

main.o: main.cpp ../srstrgen_pm/st.h

st.o: ../srstrgen_pm/st.cpp ../srstrgen_pm/st.h
	$(CXX) -c $(CXXFLAGS) ../srstrgen_pm/st.cpp -o ../srstrgen_pm/st.o -std=c++11

win_clean:
	del *.o
	del *.exe