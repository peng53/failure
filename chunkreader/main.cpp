#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	ChunkReader F("closure.txt",80);
	string str;
	//F.until('{');
	F.closure(str);
	cout << str;
	//cout << F;
	//cout << F;
	return 0;
}
