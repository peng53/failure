#include "parser.h"
#include <stdexcept>
#include <cctype>
#include <stack>

using std::stack;

template <class T>
char next_symplex(T& chr){
	// Returns next JSON recognized symbol.
	// If none is found, ChunkReader would have been read entirely, and a
	// runtine_error would be thrown.
	char c;
	while (!chr.empty()){
		c = chr.get();
		if (isdigit(c)){
			return '0';
		}
		switch (c){
			case '{':
			case '[':
			case '"':
			case ']':
			case '}':
				return c;
			case '\0':
				throw std::runtime_error("File ended prematurely.");
		}
		chr.advance();
	}
	throw std::runtime_error("File ended prematurely. :<");
}
template <class T>
string get_a_number(T& chr){
	// Grabs a doubleing point number from ChunkReader.
	// chr's current position should already point to a valid digit
	// at time of call. A decimal is allowed but numbers must start
	// with a digit. The double is returned when non-digit/EOF/2nd
	// decimal is encountered. ChunkReader's position is left at this
	// invalid char.
	char c;
	string s;
	bool found_dot = false;
	while (!chr.empty()){
		c = chr.get();
		if (!isdigit(c)){
			if (!found_dot && c=='.'){
				found_dot = true;
			} else {
				break;
			}
		}
		s += c;
		chr.advance();
	}
	return s;
	//return std::stod(s);
}
template <class T>
string* get_a_string(T& chr, string* s_ptr){
	// Grabs characters from ChunkReader until a " is encountered
	// without a preceding \. If chr's position is " at call, this
	// function returns after advancing once. If chr is consumed
	// before finding a ", a runtine_error is thrown.
	char c;
	while (!chr.empty()){
		c = chr.get();
		switch (c){
			case '"':
				chr.advance();
				return s_ptr;
			case '\\':
				chr.advance();
				if (s_ptr){
					if (chr.get()!='"'){
						(*s_ptr) += '\\';
					}
					(*s_ptr) += chr.get();
				}
				break;
			default:
				if (s_ptr){
					(*s_ptr) += c;
				}
				break;
		}
		chr.advance();
	}
	throw std::runtime_error("Closing double quote not found.");
}
template <class T>
string* get_a_string_q(T& chr,const char q,string* s_ptr){
	char c;
	while (!chr.empty()){
		c = chr.get();
		chr.advance();
		if (c==q){
			return s_ptr;
		} else if (c=='\\'){
			if (s_ptr){
				if (chr.get()!=q){
					(*s_ptr) += '\\';
				}
				(*s_ptr) += chr.get();
			}
			chr.advance();
		} else {
			if (s_ptr){
				(*s_ptr) += c;
			}
		}
	}
	throw std::runtime_error("Closing quote not found.");	
}
template <class T>
string get_a_string(T& chr){
	// Calls get_a_string without an output string.
	string str;
	get_a_string(chr,&str);
	return str;
}
template <class T>
Jso* text2obj(T& chr, JType t){
	// Returns a Jso* from data in ChunkReader given a JType.
	// calls get_a_number/string for numbers/strings.
	switch (t){
		case JType::Num:
			return new Jso(get_a_number(chr));
		case JType::Str:
			return new Jso(get_a_string(chr));
		default:
			return new Jso(t);
	}
}
JType char2type(char c){
	// Converts a char to a JType. Valid input characters are cases in
	// next_symplex. If character is not an expected one, a runtime_error
	// is thrown.
	switch (c){
		case '{': return JType::Obj;
		case '[': return JType::Arr;
		case '"': return JType::Str;
		case '0': return JType::Num;
	}
	throw std::runtime_error("Got unexpected character for type.");
}
template <class T>
void object_handler(stack<Jso*>& stk, T& chr){
	// Handles data from ChunkReader while the 'current/top' object in 'scope'
	// is an object. Usually it stops when '}' is found, or the top-object
	// is a list. Throws if: a key with 0 chars is found, an expected colon is
	// missing, a closing bracket other than '}' is found unexpectingly, or chr's
	// data runs out before a closing '}' is found.
	string key;
	char c;
	Jso* j;
	while (!chr.empty()){
		c = next_symplex(chr);
		chr.advance();
		if (c=='}'){
			stk.pop();
			return;
		}
		if (c=='"'){
			//j = text2obj(chr,mk_key_value(chr,key));
			key.clear();
			get_a_string(chr,&key);
			if (key.length()==0){
				throw std::runtime_error("Got 0-len key, which is not possible.");
			}
			if (chr.until(':')!=':'){
				throw std::runtime_error("Following colon missing in key-value pair.");
			}
			chr.advance();
			c = next_symplex(chr);
			if (c!='0'){
				chr.advance();
			}
			j = text2obj(chr,char2type(c)); // char2type will throw the exception.
			stk.top()->key_value(key,j);
			if (j->t==JType::Arr){
				stk.emplace(j);
				return;
			}
			if (j->t==JType::Obj){
				 stk.emplace(j);
			}
		} else { // it would be [{ or \0
			std::cerr << "Got " << c << '\n';
			throw std::runtime_error("Unexpected symbol encountered. Expected opening quote for key.");
		}
	}
	throw std::runtime_error("File ended prematurely.");
}
template <class T>
void array_handler(stack<Jso*>& stk, T& chr){
	// Handles data from ChunkReader while the 'current/top' object in 'scope'
	// is an array. Usually it stops when ']' is found, or the top-object
	// is an object. Throws if: a a closing bracket other than ']' is found 
	// unexpectingly, or chr's data runs out before a closing ']' is found.
	char c;
	Jso* j;
	while (!chr.empty()){
		c = next_symplex(chr);
		if (c!='0'){ // 0 would imply that char is significant
			chr.advance();
		}
		if (c==']'){
			stk.pop();
			return;
		}
		j = text2obj(chr,char2type(c)); // char2type will throw the exception.
		stk.top()->add_value(j);
		if (j->t==JType::Arr){
			stk.emplace(j);
		} else if (j->t==JType::Obj){
			stk.emplace(j);
			return;
		}
	}
	throw std::runtime_error("File ended prematurely.");
}

template <class T>
JSON& parse_file(T& chr, JSON& lv){
	stack<Jso*> stk;
	stk.emplace(*lv);
	while (!chr.empty() && !stk.empty()){
		switch (stk.top()->t){
			case JType::Obj:
				object_handler(stk,chr);
				break;
			case JType::Arr:
				array_handler(stk,chr);
				break;
			default: break;
		}
	}
	return lv;
}
template <class T>
void skipNext(T& chr, JType ft){
	// Skips objects or arrays in ChunkReader. To skip strings or numbers,
	// its best to use get_a_string/number ATM. chr's position should be
	// after the opening bracket at time of call.
	stack<JType> stk;
	stk.emplace(ft);
	char c;
	while (!stk.empty() && !chr.empty()){
		if (stk.top()==JType::Obj){
			// then there's a key to ignore. get to opening quote.
			if (chr.until('"')!='"'){
				std::cerr << "Got " << chr.get() << '\n';
				throw std::runtime_error("Unexpected symbol encountered.");
			}
			chr.advance();
			// now to skip past it.
			get_a_string(chr);
			if (chr.until(':')!=':'){
				throw std::runtime_error("Missing colon for key-value pair.");
			}
			chr.advance();
		}
		// now for the value.
		c = next_symplex(chr);
		if (c!='0'){
			chr.advance();
		}
		switch (c){
			/* Closing brackets were found. */
			case ']':
				if (stk.top()!=JType::Arr){
					std::cerr << "Got " << c << '\n';
					throw std::runtime_error("Unexpected symbol encountered.");
				}
				stk.pop();
				break;
			case '}':
				if (stk.top()!=JType::Obj){
					std::cerr << "Got " << c << '\n';
					throw std::runtime_error("Unexpected symbol encountered.");
				}
				stk.pop();
				break;
			/* That's an EOF. */
			case '\0':
				throw std::runtime_error("File ended prematurely.");
			/* An opening bracket, string, or number. */
			default:
				stk.emplace(char2type(c)); // char2type will throw the exception.
				switch (stk.top()){
					case JType::Str:
						/*if (chr.until('"')!='"'){
							throw std::runtime_error("Unexpected symbol encountered.");
						}
						chr.advance();
						// above cannot handle escapes. */
						get_a_string(chr);
						stk.pop();
						break;
					case JType::Num:
						get_a_number(chr); // get the number but discard it.
						stk.pop();
						break;
					case JType::Arr:
					case JType::Obj:
						break;
				}
				break;
		}
	}
}

//template char next_symplex<ChunkReader>(ChunkReader&);
//template JSON& parse_file<ChunkReader>(ChunkReader&, JSON&);
template char next_symplex<AReader>(AReader&);
template JSON& parse_file<AReader>(AReader&, JSON&);
