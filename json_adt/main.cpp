#include "../chunkreader/chread.h"
#include "jso_f.h"
#include <iostream>

using std::cout;
using std::cerr;

int main(){
	//ChunkReader F("../bookmarks-2018-11-30.json",80);
	ChunkReader F("eof.json",80);
	if (!F.is_open()){
		cerr << "file was not opened successfully\n";
		return 1;
	}
	if (next_symplex(F)!='{'){
		cerr << "Could not find opening curly brace.\n";
		return 1;
	}
	F.advance();
	JSON lv;
	stack<Jso*> stk;
	stk.emplace(*lv);
	try {
		while (!F.empty() && !stk.empty()){
			switch (stk.top()->t){
				case JType::Obj:
					object_handler(stk,F);
					break;
				case JType::Arr:
					array_handler(stk,F);
					break;
				default:
					break;
			}
		}
	} catch (const std::runtime_error& e) {
		cout << e.what();
		return 1;
	}
	//(*lv)->rprint(cout,"tree");
	cout << lv;
	return 0;
}
