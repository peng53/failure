/* istream ifstream tests

*/
#include "stream_test.h"
#include <sstream>
#include <ios>

using std::filebuf;
using std::ios_base;
using std::cout;
using std::istringstream;

AReader::AReader(streambuf* sb,const size_t csize):
	t(sb),
	ch(new char[csize]),
	I(0),
	E(0),
	M(csize),
	good(true)
{
	feed();
}
AReader& AReader::operator++(){
	++I;
	if (!has_data() && good){
		feed();
	}
	return *this;
}
AReader::~AReader(){
	delete ch;
}
void AReader::feed(){
	if (good){
		I = 0;
		E = t->sgetn(ch,M-1);
		ch[E] = '\0';
		if (E<M-1){
			good = false;
		}
	} else {
		I = 0;
		E = 0;
	}
}
char AReader::get(){
	return ch[I];
}
bool AReader::has_data(){
	return I<E;
}
char AReader::until(char c,string* str_ptr){
	do {
		if (get()==c){
			return get();
		}
		if (str_ptr){
			(*str_ptr) += get();
		}
		++(*this);	
	} while (has_data() || good);
	return '\0'; // this means no more data left.
}
ostream& operator<<(ostream& out,AReader& rhs){
	if (rhs.has_data()){
		out << rhs.ch+rhs.I;
		rhs.feed();
	}
	return out;
}

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
	AReader o2(f,64);
	//cout << "*** Obj Status: good-" << o2.good << " hasdata-" << o2.has_data() << '\n';
	if (o2.until('{')!='{'){
		std::cerr << "Couldn't find a '{'!\n";
		return 1;
	}
	++o2;
	/*
	string rest;
	o2.until('\0',&rest);
	cout << rest << '\n';
	*/
	cout << o2 << '\n';
	if (my_fb.is_open()){
		my_fb.close();
	}
	return 0;
}
