#include "stream_test.h"
#include <sstream>
#include <ios>

using std::filebuf;
using std::ios_base;
using std::cout;
using std::istringstream;

int main(int argc, char** argv){
	streambuf* f;
	istringstream ins_str;
	filebuf my_fb;
	if (argc<2){
		/* for string */
		cout << "Using strings. Input something or return.\n";
		string blah;
		getline(std::cin,blah);
		if (blah.length()==0){
			blah = "testing string\n";
		}
		ins_str.str(blah);
		f = ins_str.rdbuf();
	}  else {
		/* for file */
		cout << "Using file: " << argv[1] << '\n';
		my_fb.open(argv[1],ios_base::in);
		if (!my_fb.is_open()){
			std::cerr << "Couldn't open file.\n";
			return 1;
		}
		f = &my_fb;
	}
	IReader *o2 = new AReader(f,64);
	//AReader o2(f,64);
	//cout << "*** Obj Status: good-" << o2.good << " hasdata-" << o2.has_data() << '\n';
	if (o2->until('{')!='{'){
		std::cerr << "Couldn't find a '{'!\n";
		return 1;
	}
	cout << '{';
	o2->advance();
	
	string rest;
	o2->until('\0',&rest);
	cout << rest << '\n';
	
	//cout << *o2 << '\n';
	if (my_fb.is_open()){
		my_fb.close();
	}
	delete o2;
	return 0;
}
