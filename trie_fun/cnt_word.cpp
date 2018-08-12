// Count word prog

#include <iostream>
#include "wordbank.h"
#include <string>

using std::cout;

bool is_letter(char c){
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int main(int argc, char** argv){
	if (argc<1){
		return 1;
	}
	WordBank ws;
	for (int i=1;i<argc;++i){
		ws.add_word(argv[i]);
	}
	if (ws.prefix_exists("ad")){
		std::cout << "you have 'ad' as a prefix\n";
	} else {
		std::cout << "you don't have 'ad' as a prefix\n";
	}
	vector<string> r = ws.with_prefix("ad");
	for (auto s : r){
		std::cout << s << '\n';
	}
	std::cout << argv[1] << " removed\n";
	ws.remove_word(argv[1]);
	return 0;
}