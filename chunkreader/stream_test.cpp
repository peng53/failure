/* istream ifstream tests

*/
#include "stream_test.h"

int main(int argc, char** argv){
	std::streambuf* f;
	std::ifstream my_file;
	std::istringstream ins_str;
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
		my_file.open(argv[1],std::ifstream::in);
		if (!my_file.is_open()){
			std::cout << "Couldn't open file.\n";
			return 1;
		}
		f = my_file.rdbuf();
	}
	Obj o2(f,64);
	std::cout << "*** Obj Status: good-" << o2.good << " hasdata-" << o2.has_data() << '\n';
	while (o2.has_data() || o2.good){
		std::cout << o2.get();
		++o2;
	}
	std::cout << '\n';
	return 0;
}
