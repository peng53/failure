#include "chread.h"
#include <stack>
#include <iostream>
#include <stack>

using std::stack;

ChunkReader::ChunkReader(const char* filename,const size_t csize):
	ifstream(filename,std::ifstream::in),
	I(0),
	E(0),
	M(csize),
	ch(new char[csize]),
	good(true)
{
	feed();
	
}
ChunkReader::~ChunkReader(){
	delete ch;
	close();
}
void ChunkReader::feed(){
	if (good){
		read(ch,M-1);
		I = 0;
		E = gcount();
		ch[E] = '\0';
		if (E<M-1){
			good = false;
		}
	} else {
		I = 0;
		E = 0;
	}
}
char ChunkReader::until(char c,string* str_ptr){
	do {
		if (get()==c){
			return get();
		}
		if (str_ptr){
			(*str_ptr) += get();
		}
		advance();
	} while (has_data() || good);
	return '\0'; // this means no more data left.
}

string ChunkReader::capture_until(char c){
	string str;
	until(c,&str);
	return str;
}

ostream& operator<<(ostream& out,ChunkReader& rhs){
	if (rhs.E>0){
		out << string(rhs.ch+rhs.I,rhs.E-rhs.I);
		rhs.feed();
	}
	return out;
}

static map<char,char> BRACKETS = {
	{'[',']'}, {']','['},
	{'{','}'}, {'}','{'},
	{'"','"'}, 
};

string& ChunkReader::closure(string& s){
	// Looks at current char, if its in BRACKETS,
	// collects all characters to string until
	// its counterpart is encountered.
	char c = ch[I];
	++I;
	if (I==E){
		feed();
	}
	if (BRACKETS.count(c)==0){
		return s;
	}
	stack<char> stk;
	stk.emplace(BRACKETS[c]);
	while (E!=0 && !stk.empty()){
		c = ch[I];
		if (BRACKETS.count(c)>0){
			if (c==stk.top()){
				stk.pop();
				if (stk.empty()){
					return s;
				}
			} else {
				stk.emplace(BRACKETS[c]);
			}
		}
		s += c;
		++I;
		if (I==E){
			feed();
		}
	}
	return s;
}
string ChunkReader::closure(){
	string str;
	return closure(str);
}
void ChunkReader::advance(){
	++I;
	if (!has_data() && good){
		feed();
	}
}
char ChunkReader::get(){
	return ch[I];
}
bool ChunkReader::empty(){
	return (E==0 && ifstream::eof());
}
bool ChunkReader::has_data(){
	return I<E;
}
