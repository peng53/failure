#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	IReader *F = new ChunkReader("../in_out/test_json.json",80);
	//ChunkReader F("../in_out/test_json.json",80);
	/*
	if (!F->is_open()){
		cout << "Couldn't open file.\n";
		return 1;
	}
	*/
	string str;
	F->until('{');
	F->until('"');
	F->advance();
	F->until('"',&str);
	cout << str << '\n';
	delete F;
	return 0;
}
