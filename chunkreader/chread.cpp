#include "chread.h"
#include <iostream>

ChunkReader::ChunkReader(const size_t csize):
	ifstream(),
	M(csize),
	I(0),
	E(0),
	ch(new char[csize])
{
}

ChunkReader::ChunkReader(const string& filename,const size_t csize):
	ChunkReader(csize)
{
	load(filename);
}

void ChunkReader::load(const string& filename){
	if (is_open()){
		close();
	}
	ifstream::open(filename,std::ifstream::in);
	IReader::good = true;
	feed();
}

ChunkReader::~ChunkReader(){
	std::cout << "BOOM!\n";
	delete ch;
	close();
}
void ChunkReader::feed(){
	if (IReader::good){
		read(ch,M-1);
		I = 0;
		E = gcount();
		ch[E] = '\0';
		if (E<M-1){
			IReader::good = false;
		}
	} else {
		I = 0;
		E = 0;
	}
}
char ChunkReader::until(const char c,string* str_ptr){
	do {
		if (get()==c){
			return get();
		}
		if (str_ptr){
			(*str_ptr) += get();
		}
		advance();
	} while (has_data() || IReader::good);
	return '\0'; // this means no more data left.
}
ChunkReader& ChunkReader::operator++(){
	advance();
	return *this;
}
char ChunkReader::get(){
	return ch[I];
}
bool ChunkReader::empty(){
	return !(has_data() || IReader::good);
}
bool ChunkReader::has_data(){
	return I<E;
}
ostream& operator<<(ostream& out,ChunkReader& rhs){
	if (rhs.has_data()){
		out << rhs.ch+rhs.I;
		rhs.feed();
	}
	return out;
}
void ChunkReader::advance(){
	++I;
	if (!has_data() && IReader::good){
		feed();
	}
}
