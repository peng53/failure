#include "chread.h"
#include <stack>
#include <map>

using std::stack;
using std::map;

ChunkReader::ChunkReader(const char* filename,const size_t csize):
	IReader(csize),
	ifstream(filename,std::ifstream::in),
	I(0),
	E(0),
	ch(new char[csize])
{
	feed();
}
ChunkReader::~ChunkReader(){
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
char ChunkReader::until_e(const char end,const bool int_escape,string* str_ptr){
	bool escape = false;
	char c;
	do {
		if (escape){
			escape = false;
			if (str_ptr){
				(*str_ptr) += get();
			}
		} else {
			c = get();
			if (int_escape && c=='\\'){
				escape = true;
			} else if (c==end){
				return c;
			}
			if (str_ptr){
				(*str_ptr) += get();
			}
		}
		advance();
	} while (has_data() || IReader::good);
	return '\0';
}
string ChunkReader::capture_until(const char c){
	string str;
	until(c,&str);
	return str;
}

ostream& operator<<(ostream& out,ChunkReader& rhs){
	if (rhs.has_data()){
		out << rhs.ch+rhs.I;
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
	char c = get();
	if (BRACKETS.count(c)==0){
		return s;
	}
	advance();
	stack<char> stk;
	stk.emplace(BRACKETS[c]);
	while (!stk.empty() && (has_data() || IReader::good)){
		c = get();
		if (BRACKETS.count(c)>0){
			if (c==stk.top()){
				if (stk.size()<=1){
					return s;
				} else {
					stk.pop();
				}
			} else {
				stk.emplace(BRACKETS[c]);
			}
		}
		s += c;
		advance();
	}
	return s;
}
string ChunkReader::closure(){
	string str;
	return closure(str);
}
void ChunkReader::advance(){
	++I;
	if (!has_data() && IReader::good){
		feed();
	}
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
