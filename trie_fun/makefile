CFLAGS=-pedantic -O2 -march=native
CC=clg++

cnt_word.o: cnt_word.cpp
	$(CC) $(CFLAG) -c cnt_word.cpp

wordbank.o: wordbank.cpp
	$(CC) $(CFLAG) -c wordbank.cpp

cnt_word: cnt_word.o wordbank.o
	$(CC) $(CFLAG) cnt_word.o wordbank.o

main: wordbank.o ../srstrgen_pm/st.o main.o
	$(CC) $(CFLAG) main.o wordbank.o ../srstrgen_pm/st.o -o main
st.o: ../srstrgen_pm/st.cpp ../srstrgen_pm/st.h
	$(CC) $(CFLAG) -c ../srstrgen_pm/st.cpp -o ../srstrgen_pm/st.o -std=c++11
main.o: main.cpp
	$(CC) $(CFLAG) -c main.cpp

clean:
	del *.o
	del *.exe