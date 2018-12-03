#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	//ChunkReader F("../test_json.json",80);
	ChunkReader F("",80);
	//cout << std::cin.rdbuf();
	//F.rdbuf(std::cin.rdbuf());
	string str;
	//F.until('{');
	//F.until('"');
	//F.closure(str);
	cout << str;
	//cout << F;
	//cout << F;
	return 0;
}
