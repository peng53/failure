#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	ChunkReader f("../test.json",1024);
	char c;
	while (!f.empty()){
		c = f.at();
		cout << c;
		f.advance();
	}
	return 0;
}
