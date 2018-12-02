#include "parser.h"
#include <stdexcept>
#include <cctype>
#include <stack>

using std::stack;

char next_symplex(ChunkReader& chr){
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
float get_a_number(ChunkReader& chr){
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
	return std::stof(s);
}
string& get_a_string(ChunkReader& chr, string& s){
	char c;
	while (!chr.empty()){
		c = chr.get();
		switch (c){
			case '"':
				chr.advance();
				return s;
			case '\\':
				chr.advance();
				if (chr.get()=='"'){
					s += '"';
				} else {
					s += '\\';
					s += chr.get();
				}
				break;
			default:
				s += c;
				break;
		}
		chr.advance();
	}
	throw std::runtime_error("Closing double quote not found.");	
}
string get_a_string(ChunkReader& chr){
	string str;
	get_a_string(chr,str);
	return str;
}
Jso* text2obj(ChunkReader& chr, JType t){
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
	switch (c){
		case '{': return JType::Obj;
		case '[': return JType::Arr;
		case '"': return JType::Str;
		case '0': return JType::Num;
	}
	throw std::runtime_error("Got unexpected character for type.");
}
void object_handler(stack<Jso*>& stk, ChunkReader& chr){
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
			get_a_string(chr,key);
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
			throw std::runtime_error("Unexpected symbol encountered.");
		}
	}
	throw std::runtime_error("File ended prematurely.");
}
void array_handler(stack<Jso*>& stk, ChunkReader& chr){
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

JSON& parse_file(ChunkReader& chr, JSON& lv){
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
