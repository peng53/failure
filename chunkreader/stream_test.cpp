/* istream ifstream tests

*/
#include "stream_test.h"

int main(){
//	std::filebuf* f;
//	std::ifstream myfile("../in_out/eof.json",std::ifstream::in);
	std::stringbuf* f;
	std::istringstream myfile;
	std::string blah = "test";
	myfile.str(blah);
	f = myfile.rdbuf();
	//std::cout << (char)f->sgetc();
	Obj o2(f,64);
	std::cout << o2.good << o2.has_data << '\n';
	while (o2.has_data || o2.good){
		std::cout << o2.get();
		++o2;
	}
	std::cout << '\n';
	return 0;
}
