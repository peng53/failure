#include "chread.h"
#include <stack>

using std::stack;

ChunkReader::ChunkReader(const char* filename,const size_t csize):
	ifstream(filename,std::ifstream::in),
	i(0),
	b(0),
	S(csize){
		ch = new char[S];
		feed();
}
ChunkReader::~ChunkReader(){
	delete[] ch;
	close();
}
void ChunkReader::feed(){
	get(ch,S);
	i = 0;
}
string ChunkReader::get_capture(){
	return string(ch+b,ch+i);
}
string& ChunkReader::get_capture(string& s){
	return s+= get_capture();
}

string& ChunkReader::capture_until(string& s, char c){
	b = i;
	while (!eof() && ch[i]!=c){
		if (i+1==S){
			get_capture(s);
			feed();
			b = 0;
		} else {
			++i;
		}
	}
	get_capture(s);
	advance();
	return s;
}
string ChunkReader::capture_until(char c){
	string s;
	return capture_until(s,c);
}

static map<char,char> BRACKETS = {
	{'[',']'}, {']','['},
	{'{','}'}, {'}','{'},
	{'"','"'}, 
};

string& ChunkReader::closure(string& s){
	char c = at();
	advance();
	if (BRACKETS.count(c)==0){
		return s;
	}
	stack<char> stk;
	stk.emplace(BRACKETS[c]);
	while (!eof() && !stk.empty()){
		c = at();
		if (BRACKETS.count(c)==0){
			s += c;
		} else {
			if (c==stk.top()){
				stk.pop();
			} else {
				stk.emplace(BRACKETS[c]);
			}
		}
		advance();
	}
	return s;
}
char ChunkReader::at(){
	return ch[i];
}
void ChunkReader::advance(){
	++i;
	if (i==S) feed();
}
bool ChunkReader::empty(){
	return at()=='\0' && eof();
}
