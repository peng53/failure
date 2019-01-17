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

static unordered_map<char,char> BRACKETS = {
	{'[',']'}, {']','['},
	{'{','}'}, {'}','{'},
	{'"','"'}, 
};

template <typename T>
T& closure(IReader* reader,T& s,unordered_map<char,char>* match){
	// Looks at current char, if its in BRACKETS,
	// collects all characters to string until
	// its counterpart is encountered.
	if (reader->empty()){
		return s;
	}
	if (!match){
		match = &BRACKETS;
	}
	char c = reader->get();
	if (match->count(c)==0){
		return s;
	}
	reader->advance();
	stack<char> stk;
	stk.emplace(match->at(c));
	while (!stk.empty() && (!reader->empty())){
		c = reader->get();
		if (match->count(c)>0){
			if (c==stk.top()){
				if (stk.size()<=1){
					return s;
				} else {
					stk.pop();
				}
			} else {
				stk.emplace(match->at(c));
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
