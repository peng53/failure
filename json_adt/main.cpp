#include "parser.h"
#include <iostream>

using std::cout;
using std::cerr;

int main(int argc, char** argv){
	if (argc<2){
		cerr << "Missing input json file\n";
		return 1;
	}
	ChunkReader F(argv[1],80);
	if (!F.is_open()){
		cerr << "File was not opened successfully\n";
		return 1;
	}
	if (next_symplex(F)!='{'){
		cerr << "Could not find opening curly brace.\n";
		return 1;
	}
	F.advance();
	JSON lv;
	try {
		parse_file(F,lv);
	} catch (const std::runtime_error& e) {
		cerr << e.what()
			<< "\nStructure incomplete. Printing what was recieved:\n"
			<< lv;
		return 1;
	}
	cout << lv;
	return 0;
}
