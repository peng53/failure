/* istream ifstream tests

*/
#include "stream_test.h"
#include <sstream>
#include <ios>
#include <string>

using std::ios_base;

int main(int argc, char** argv){
	std::streambuf* f;
	std::istringstream ins_str;
	std::filebuf my_fb;
	if (argc<2){
		/* for string */
		std::cout << "Using strings. Input something or return.\n";
		std::string blah;
		std::getline(std::cin,blah);
		if (blah.length()==0){
			blah = "testing string\n";
		}
		ins_str.str(blah);
		f = ins_str.rdbuf();
	}  else {
		/* for file */
		std::cout << "Using file: " << argv[1] << '\n';
		my_fb.open(argv[1],ios_base::in);
		if (!my_fb.is_open()){
			std::cerr << "Couldn't open file.\n";
			return 1;
		}
		f = &my_fb;
	}
	Obj o2(f,64);
	std::cout << "*** Obj Status: good-" << o2.good << " hasdata-" << o2.has_data() << '\n';
	while (o2.has_data() || o2.good){
		std::cout << o2.get();
		++o2;
	}
	/* if file was used */
	if (my_fb.is_open()){
		my_fb.close();
	}
	std::cout << '\n';
	return 0;
}
