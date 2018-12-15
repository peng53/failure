#include "chread.h"
#include <stack>
#include <map>

using std::stack;
using std::map;

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
	while (!stk.empty() && (has_data() || good)){
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
	if (!has_data() && good){
		feed();
	}
}
char ChunkReader::get(){
	return ch[I];
}
bool ChunkReader::empty(){
	return !(has_data() || good);
}
bool ChunkReader::has_data(){
	return I<E;
}
