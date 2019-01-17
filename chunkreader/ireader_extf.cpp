#include "ireader_extf.h"
#include <stack>

using std::stack;

void reader2stdout(IReader* reader,ostream& out){
	while (!reader->empty()){
		out << reader->get();
		reader->advance();
	}
	out << '\n';
}

static map<char,char> BRACKETS = {
	{'[',']'}, {']','['},
	{'{','}'}, {'}','{'},
	{'"','"'}, 
};

string& closure(IReader* reader,string& s){
	// Looks at current char, if its in BRACKETS,
	// collects all characters to string until
	// its counterpart is encountered.
	if (reader->empty()){
		return s;
	}
	char c = reader->get();
	if (BRACKETS.count(c)==0){
		return s;
	}
	reader->advance();
	stack<char> stk;
	stk.emplace(BRACKETS[c]);
	while (!stk.empty() && (!reader->empty())){
		c = reader->get();
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
		reader->advance();
	}
	return s;
}
string closure(IReader* reader){
	string output;
	return closure(reader,output);
}
