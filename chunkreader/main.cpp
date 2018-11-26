#include "chread.h"
#include "../json_adt/jso_f.h"
#include <iostream>

using std::cout;

int main(){
	ChunkReader F("../test_json.json",80);
	JSON lv;
	string str;
	F.until('{');
	F.until('"');
	F.closure(str);
	cout << str;
	//cout << F;
	//cout << F;
	return 0;
}
