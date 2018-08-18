cnt_word_old: cnt_word.o wordbank.o
	clg++ cnt_word.o wordbank.o

cnt_word.o: cnt_word.cpp
	clg++ -c cnt_word.cpp

wordbank.o: wordbank.cpp wordbank.h
	clg++ -c wordbank.cpp

cnt_word: cnt_word.cpp wordbank.cpp wordbank.h
	clg++ cnt_word.cpp wordbank.cpp