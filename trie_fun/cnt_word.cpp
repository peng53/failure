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
	string test_phrase = "cat car bat bar bowel bowl young you";
	size_t first_letter;
	WordBank ws;
	/*
	for (size_t i=0,l=test_phrase.length();i<l;++i){
		if (is_letter(test_phrase[i])){
			first_letter = i;
			++i;
			while (i<l && is_letter(test_phrase[i])){
				++i;
			}
			ws.add_word(test_phrase.substr(first_letter,i-first_letter));
		}
	}
	*/
	for (int i=1;i<argc;++i){
		ws.add_word(argv[i],strlen(argv[i]));
	}
	/*
	if (ws.prefix_exists("bat",3)){
		cout << "'bat' is in the wordbank\n";
	}
	if (ws["bat"]){
		cout << "'bat' is in the wordbank!\n";
	}
	string a = "thi";
	vector<string> r = ws.as_vector();
	for (auto s : r){
		cout << s << '\n';
	}
	ws.remove_word("young");
	r = ws.as_vector();
	cout<<"words again\n";
	for (auto s : r){
		cout << s << '\n';
	}
	*/
	ws.dump_levels(cout);
	
	return 0;
}