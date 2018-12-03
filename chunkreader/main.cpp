#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	ChunkReader F("../in_out/test_json.json",80);
	if (!F.is_open()){
		cout << "Couldn't open file.\n";
		return 1;
	}
	string str;
	F.until('{');
	F.until('"');
	F.closure(str);
	cout << str;
	return 0;
}
