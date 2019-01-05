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
bool AReader::empty(){
	return !(has_data() || good);
}
