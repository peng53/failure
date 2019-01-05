#include "parser.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::cerr;
using std::istringstream;
using std::filebuf;
using std::ios_base;

int main(int argc, char** argv){
	streambuf* f;
	istringstream ins_str;
	filebuf my_fb;
	if (argc<2){
		cout << "No input file. Taking input from stdin.\n";
		string userInput;
		getline(std::cin,userInput);
		if (userInput.length()==0){
			cout << "Nothing was entered.\nQuiting program.\n";
			return 1;
		}
		ins_str.str(userInput);
		f = ins_str.rdbuf();
	} else {
		cout << "Using file: "
			<< argv[1]
			<< '\n';
		my_fb.open(argv[1],ios_base::in);
		if (!my_fb.is_open()){
			cout << "Could not open file.\n";
			return 1;
		}
		f = &my_fb;
	}
	AReader textChunk(f,1024);
	if (next_symplex(textChunk)!='{'){
		cerr << "Could not find opening curly brace.\n";
		return 1;
	}
	textChunk.advance();
	JSON jsonTree;
	try {
		parse_file(textChunk,jsonTree);
	} catch (const std::runtime_error& e) {
		cerr << e.what()
			<< "\nStructure incomplete. Printing what was recieved:\n"
			<< jsonTree;
		return 1;
	}
	cout << jsonTree;
	if (my_fb.is_open()){
		my_fb.close();
	}
	return 0;
}
