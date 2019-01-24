#include "parser.h"
#include <stdexcept>
#include <cctype>
#include <stack>
#include <cctype>
//#include <iostream>

//using std::cout;
using std::stack;
using std::pair;

char nextNonWS(IReader* buf){
	while (!buf->empty() && isspace(buf->get())){
		buf->advance();
	}
	return buf->get();
}

char verifySymbol(char c){
	// Checks whether c is notable char.
	// Returning it or the group.
	// If check fails, an exception is thrown.
	if (isdigit(c)){
		return '0';
	}
	switch (c){
		case 'n': case 't': case 'f':
		case '{': case '}':
		case '[': case ']':
		case '"':
			return c;

		case '-': return '0';
		case '\0': throw std::runtime_error("File ended prematurely.");
	}
	throw std::runtime_error("File ended prematurely. :<");
}

static void notNullAppend(string* out, char c){
	if (out){
		(*out) += c;
	}
}

static void parseDigits(IReader* buf, bool decimal, string* out=nullptr){
	// Grabs a decimal point number from IReader putting them into
	// out. buf's inital state should be at a character while its
	// final state will be preceding the last character.
	if (buf->empty()){
		return;
	}
	char c = buf->get();
	if (!(isdigit(c) || c=='-')){
		return;
	}
	notNullAppend(out,c);
	buf->advance();
	while (!buf->empty()){
		c = buf->get();
		if (!isdigit(c)){
			if (c=='.' && decimal){
				decimal = false;
			} else if (c=='e'){
				notNullAppend(out,'e');
				buf->advance();
				// belows parses only the 'e' part as int.
				return parseDigits(buf,false,out);
			} else {
				// end of number.
				break;
			}
		}
		notNullAppend(out,c);
		buf->advance();
	}
}

static void get_a_string(IReader* chr, string* s_ptr){
	// Grabs characters from ChunkReader until a " is encountered
	// without a preceding \. If chr's position is " at call, this
	// function returns after advancing once. If chr is consumed
	// before finding a ", a runtine_error is thrown.
	for (char c; !chr->empty(); chr->advance()){
		c = chr->get();
		switch (c){
			case '"':
				chr->advance();
				return;
			case '\\':
				chr->advance();
				if (chr->get()!='"'){
					notNullAppend(s_ptr,'\\');
				}
				notNullAppend(s_ptr,c);
				break;
			default:
				notNullAppend(s_ptr,c);
				break;
		}
	}
	throw std::runtime_error("Closing double quote not found.");
}

static bool next_chars_are(IReader* buf, const string& chars){
	// buf should start at chars[0]
	for (auto c : chars){
		if (buf->empty() || c!=buf->get()){
			return false;
		}
		buf->advance();
	}
	// we got to the end of chars!
	return true;
}

static Jso* text2obj(IReader* chr, JType t,JSON& tree){
	// Returns a Jso* from data in ChunkReader given a JType.
	// calls get_a_number/string for numbers/strings.
	string tmp;
	switch (t){
		case JType::Num:
			parseDigits(chr,true,&tmp);
			return tree.Str(tmp);
			// get a number actually returns a string.
		case JType::Str:
			get_a_string(chr,&tmp);
			return tree.Str(tmp);
		case JType::Null:
			if (next_chars_are(chr,"ull")){
				return tree.Null();
			}
			break;
		case JType::True:
			if (next_chars_are(chr,"rue")){
				return tree.True();
			}
			break;
		case JType::False:
			if (next_chars_are(chr,"alse")){
				return tree.False();
			}
			break;
		case JType::Obj:
			return tree.Map();
		case JType::Arr:
			return tree.Arr();
	}
	throw std::runtime_error("Expected Json object.");
}
static JType char2type(char c){
	// Converts a char to a JType. Valid input characters are cases in
	// next_symplex. If character is not an expected one, a runtime_error
	// is thrown.
	switch (c){
		case '{': return JType::Obj;
		case '[': return JType::Arr;
		case '"': return JType::Str;
		case '0': return JType::Num;
		case 'n': return JType::Null;
		case 't': return JType::True;
		case 'f': return JType::False;
	}
	//cout << c;
	throw std::runtime_error("Got unexpected character for type.");
}

Jso* valueFromReader(IReader* buf, JSON& builder){
	// Parses input for next value.
	// buf's initial state should be at first character of value.
	char c = verifySymbol(buf->get());
	if (c!='0'){
		buf->advance();
	}
	return text2obj(buf, char2type(c), builder);
}
pair<string, Jso*> keyValueFromReader(IReader* buf, JSON& builder){
	// Parses input for next key-value.
	// buf's initial state should be at the '"'.
	pair<string, Jso*> R;
	if (buf->get()!='"'){
		//cout << buf->get();
		throw std::logic_error("Expected opening double quote but got different.");
	}
	buf->advance();
	get_a_string(buf,&(R.first));
	if (R.first.length()==0){
		throw std::runtime_error("Got 0-len key, which is not possible.");
	}
	//cout << R.first << '\n';
	//buf->advance();
	if (nextNonWS(buf)!=':'){
		//cout << buf->get();
		throw std::runtime_error("Following colon missing in key-value pair.");
	}
	buf->advance();
	R.second = valueFromReader(buf,builder);
	if (!R.second){
		throw std::runtime_error("Did not get value from reader.");
	}
	return R;
}
static bool closing_match(JType TYPE, char c){
	if (TYPE==JType::Arr){
		return c==']';
	} else if (TYPE==JType::Obj){
		return c=='}';
	}
	return false;
}

/*
static bool matches_type(IReader* buf,JType TYPE){
	string to_check;
	if (t==JType::Null){
		to_check = "ull";
	} else if (t==JType::True){
		to_check = "rue";
	} else if (t==JType::False){
		to_check = "alse";
	} else {
		return false;
	}
	return next_chars_are(buf, to_check);
}

*/
JSON& parse_file_comma(IReader* buf, JSON& tree){
	stack<Jso*> stk;
	stk.emplace(*tree);
	char c;
	pair<string,Jso*> key_value;
	bool expects_comma = false;
	while (!stk.empty() && !buf->empty()){
		// if Obj is on top, need to get key first.
		c = nextNonWS(buf);
		if (closing_match(stk.top()->t,c)){
			expects_comma = true;
			buf->advance();
			stk.pop();
			continue;
		}
		if (expects_comma){
			if (c!=','){
				throw std::logic_error("Missing comma after value.");
			}
			expects_comma = false;
			buf->advance();
			c = nextNonWS(buf);
		}
		if (stk.top()->t==JType::Obj){
			key_value = keyValueFromReader(buf,tree);
			if (key_value.second){
				stk.top()->Append(key_value);
			}
		} else if (stk.top()->t==JType::Arr){
			key_value.second = valueFromReader(buf,tree);
			if (key_value.second){
				stk.top()->Append(key_value.second);
			}
		} else {
			
		}
		if (key_value.second->t==JType::Obj || key_value.second->t==JType::Arr){
			stk.emplace(key_value.second);
			continue;
		}
		// Has a following value.
		expects_comma = true;
	}
	if (!stk.empty()){
		throw std::runtime_error("File ended prematurely.");
	}
	return tree;
}