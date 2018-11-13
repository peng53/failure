#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	/*ChunkReader f("main.cpp",24);
	for (size_t i=0;i<24;++i){
		cout << f.at();
		f.advance();
	}*/
	ifstream F ("main.cpp",std::ifstream::in);
	char t[64];
	size_t i;
	for (i=0;(i<64 && F.good()); ++i){
		F.get(t[i]);
	}
	for (size_t j=0;(j<i); ++j){
		cout << t[j];
	}
	F.close();
	return 0;
}