#include "chread.h"
#include <stack>
#include <iostream>
#include <stack>

using std::stack;

ChunkReader::ChunkReader(const char* filename,const size_t csize):
	ifstream(filename,std::ifstream::in),
	I(0),
	E(0),
	M(csize)
{
	ch = new char[M];
	feed();
}
ChunkReader::~ChunkReader(){
	delete ch;
	close();
}
void ChunkReader::feed(){
	if (good()){
		read(ch,M-1);
		I = 0;
		E = gcount();
		ch[E] = '\0';
	} else {
		I = 0;
		E = 0;
	}
}
char ChunkReader::until(char c){
	while (E>0){
		while (I<E){
			if (ch[I]==c){
				return ch[I];
			}
			++I;
		}
		feed();
	}
	std::cerr << "GOT TO EOF!\n";
	return '\0';
}
string ChunkReader::capture_until(char c){
	string str;
	return capture_until(str,c);
}
string& ChunkReader::capture_until(string& str,char c){
	while (good() || I<E){
		for (;I<E;++I){
			if (ch[I]==c) return str;
			str += ch[I];
		}
		if (good()) feed();
	}
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
	if (I>=E && good()){
		feed();
	}
}
