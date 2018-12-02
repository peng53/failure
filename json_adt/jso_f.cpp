#include "jso_f.h"
#include <stdexcept>

using std::cout;
using std::pair;

void dispose(Jso* j, stack<Jso*>& more){
	switch (j->t){
		case JType::Arr:
			for (auto &a : *(j->x.a)){
				more.emplace(a);
			}
			break;
		case JType::Obj:
			for (auto &m : *(j->x.m)){
				more.emplace(m.second);
			}
			break;
		default: break;
	}
	delete j;
}

JSON::JSON(): o(new Jso(JType::Obj)){}
JSON::~JSON(){
	stack<Jso*> D;
	Jso* d = o;
	D.emplace(d);
	while (!D.empty()){
		d = D.top();
		D.pop();
		dispose(d,D);
	}
}
JSON::JSON(const JSON& rhs): JSON(){
	// deep copy constructor
	Jso* r = rhs.o;
	Jso* l = o;
	Jso* t;
	stack<pair<Jso*,Jso*>> stk;
	stk.emplace(r,l);
	while (!stk.empty()){
		r = stk.top().first;
		l = stk.top().second;
		stk.pop();
		for (auto v : *(r->x.m)){
			switch (v.second->t){
				case JType::Str:
					(*(l->x.m))[v.first] = new Jso(*(v.second->x.s));
					// Assign to map a new string:string
					break;
				case JType::Num:
					(*(l->x.m))[v.first] = new Jso(v.second->x.f);
					// x.f is just a float, so no dereferencing needed.
					break;
				case JType::Obj:
					t = new Jso(JType::Obj);
					(*(l->x.m))[v.first] = t;
					stk.emplace(v.second,t);
					break;
				case JType::Arr:
					t = new Jso(JType::Arr);
					(*(l->x.m))[v.first] = t;
					break;
			}
		}
	}
}
Jso* JSON::operator*(){
	return o;
}

char next_symplex(ChunkReader& chr){
	char c;
	while (!chr.empty()){
		c = chr.get();
		if (c>='0' && c<='9'){
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
				throw std::runtime_error("File ended prematurely. :<");
				break;
			default:
				chr.advance();
				break;
		}
	}
	return '\0';
}
float get_a_number(ChunkReader& chr){
	char c;
	string s;
	bool found_dot = false;
	while (!chr.empty()){
		c = chr.get();
		if (c<'0' || c>'9'){
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
	return s;
}
string get_a_string(ChunkReader& chr){
	string str;
	get_a_string(chr,str);
	return str;
}
/*
JType mk_key_value(ChunkReader& chr, string& s){
	// Given a ChunkReader at '"' char, modify propname, and return a JType
	s.clear();
	chr.advance();
	chr.capture_until(s,'"');
	if (chr.get()!='"'){ // then it must be a '\0', e.g EOF
		throw std::runtime_error("File ended prematurely.");
	}
	if (s.length()==0){
		throw std::runtime_error("Got 0-len key, which is not possible.");
	}
	if (chr.until(':')!=':'){
		throw std::runtime_error("Following colon missing in key-value pair.");
	}
	chr.advance();
	switch (next_symplex(chr)){
		case '"':
			chr.advance();
			return JType::Str;
			break;
		case '{':
			chr.advance();
			return JType::Obj;
			break;
		case '[':
			chr.advance();
			return JType::Arr;
			break;
		case '0':
			return JType::Num;
			break;
		default:
			throw std::runtime_error("File ended prematurely.");
			break;
	}
	throw std::runtime_error("File ended prematurely.");
	return JType::Str;
}
*/
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
/*
string& keyNcolon(ChunkReader& chr, string& key){
	// Starting at first character (after '"')
	// keys cannot contain escapes..otherwise we would use get a string?
	key.clear();
	chr.capture_until('"');
	if (chr.get()!='"'){
		throw std::runtime_error("File ended prematurely.");
	}
	if (key.length()==0){
		throw std::runtime_error("Got 0-len key, which is not possible.");
	}
	// now to skip past the colon.
	if (chr.until(':')!=':'){
		throw std::runtime_error("Following colon missing in key-value pair.");
	}
	chr.advance();
	return key;
}
*/
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
ostream& operator<<(ostream& out,const JSON& rhs){
	rhs.o->rprint(out,"Object");
	return out;
}
