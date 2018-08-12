// Count word prog

#include <iostream>
#include "wordbank.h"
#include <string>

using std::cout;

bool is_letter(char c){
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int main(int argc, char** argv){
	if (argc<2){
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
	if (ws["cat"]){
		std::cout << "you had cat!\n";
	} else {
		std::cout << "you didn't have cat!\n";
	}
	vector<char> l = ws.next_possible_letters("ad");
	for (auto s : l){
		std::cout << s << '\n';
	}
	array<bool,26> ls = ws.next_tf("ad");
	for (char i='a';i<='z';++i){
		if (ls[i-97]){
			std::cout << i << " is viable.\n";
		}
	}
	return 0;
}