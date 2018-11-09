#include "chread.h"
#include <iostream>

using std::cout;

int main(){
	ChunkReader f("../test.json",24);
	char l[45];
	f.get(l,45);
	cout << l << '\n';
	f.get(l,45);
	cout << l << '\n';
	return 0;
}
