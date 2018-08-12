// Count word prog

#include <iostream>
#include <cstring>
#include "wordbank.h"

#include <string>
using std::cout;

bool is_letter(char c){
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int main(int argc, char** argv){
	WordBank ws;
	for (int i=1;i<argc;++i){
		ws.add_word(argv[i],strlen(argv[i]));
	}
	return 0;
}