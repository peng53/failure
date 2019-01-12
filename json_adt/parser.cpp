#include "parser.h"
#include <stdexcept>
#include <cctype>
#include <stack>

using std::stack;

char next_symplex(IReader* chr){
	// Returns next JSON recognized symbol.
	// If none is found, ChunkReader would have been read entirely, and a
	// runtine_error would be thrown.
	char c;
	while (!chr->empty()){
		c = chr->get();
		if (isdigit(c)){
			return '0';
		}
		switch (c){
			case '{':
			case '[':
			case '"':
			case ']':
			case '}':
			case 'n':
			case 't':
			case 'f':
				return c;
			case '\0':
				throw std::runtime_error("File ended prematurely.");
		}
		chr->advance();
	}
	throw std::runtime_error("File ended prematurely. :<");
}

static string get_a_number(IReader* chr){
	// Grabs a doubleing point number from ChunkReader.
	// chr's current position should already point to a valid digit
	// at time of call. A decimal is allowed but numbers must start
	// with a digit. The double is returned when non-digit/EOF/2nd
	// decimal is encountered. ChunkReader's position is left at this
	// invalid char.
	char c;
	string s;
	bool found_dot = false;
	while (!chr->empty()){
		c = chr->get();
		if (!isdigit(c)){
			if (!found_dot && c=='.'){
				found_dot = true;
			} else {
				break;
			}
		}
		s += c;
		chr->advance();
	}
	return s;
	//return std::stod(s);
}

static string* get_a_string(IReader* chr, string* s_ptr){
	// Grabs characters from ChunkReader until a " is encountered
	// without a preceding \. If chr's position is " at call, this
	// function returns after advancing once. If chr is consumed
	// before finding a ", a runtine_error is thrown.
	char c;
	while (!chr->empty()){
		c = chr->get();
		switch (c){
			case '"':
				chr->advance();
				return s_ptr;
			case '\\':
				chr->advance();
				if (s_ptr){
					if (chr->get()!='"'){
						(*s_ptr) += '\\';
					}
					(*s_ptr) += chr->get();
				}
				break;
			default:
				if (s_ptr){
					(*s_ptr) += c;
				}
				break;
		}
		chr->advance();
	}
	throw std::runtime_error("Closing double quote not found.");
}

static string get_a_string(IReader* chr){
	// Calls get_a_string without an output string.
	string str;
	get_a_string(chr,&str);
	return str;
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

static Jso* text2obj(IReader* chr, JType t){
	// Returns a Jso* from data in ChunkReader given a JType.
	// calls get_a_number/string for numbers/strings.
	switch (t){
		case JType::Num:
			return new Jso(get_a_number(chr));
		case JType::Str:
			return new Jso(get_a_string(chr));
		case JType::Null:
			if (next_chars_are(chr,"ull")){
				return &(Jso::JSO_NULL);
			}
			break;
		case JType::True:
			if (next_chars_are(chr,"rue")){
				return &(Jso::JSO_TRUE);
			}
			break;
		case JType::False:
			if (next_chars_are(chr,"alse")){
				return &(Jso::JSO_FALSE);
			}
			break;
		default: // for Obj or Arr
			return new Jso(t);
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
	throw std::runtime_error("Got unexpected character for type.");
}

static Jso* get_next_prop(IReader* buf){
	// returns a Jso* of the next object.
	char c = next_symplex(buf);
	if (c!='0'){
		buf->advance();
	}
	return text2obj(buf,char2type(c));
}

JSON& parse_file(IReader* buf, JSON& tree){
	stack<Jso*> stk;
	stk.emplace(*tree);
	char c;
	Jso* j;
	string key;
	while (!stk.empty() && !buf->empty()){
		// if Obj is on top, need to get key first.
		if (stk.top()->t==JType::Obj){
			c = next_symplex(buf);
			buf->advance();
			if (c=='}'){
				stk.pop();
				continue;
			}
			if (c=='"'){
				key.clear();
				get_a_string(buf,&key);
				if (key.length()==0){
					throw std::runtime_error("Got 0-len key, which is not possible.");
				}
				// got it! now for the colon!
				if (buf->until(':')!=':'){
					throw std::runtime_error("Following colon missing in key-value pair.");
				}
				buf->advance();
			} else {
				throw std::runtime_error("Missing opening \" for key.");
			}
		}
		c = next_symplex(buf);
		if (c==']'){
			if (stk.top()->t==JType::Arr){
				stk.pop();
				buf->advance();
				continue;
			} else {
				throw std::runtime_error("Found closing ] without opening [");
			}
		}
		// get a value
		j = get_next_prop(buf);
		if (stk.top()->t==JType::Obj){
			stk.top()->key_value(key,j);
		} else {
			stk.top()->add_value(j);
		}
		if (j->t==JType::Arr || j->t==JType::Obj){
			stk.emplace(j);
		}

	}
	if (!stk.empty()){
		throw std::runtime_error("File ended prematurely.");
	}
	return tree;
}
