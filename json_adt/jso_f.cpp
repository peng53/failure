#include "jso_f.h"

using std::cout;

void dispose(Jso* j, stack<Jso*>& more){
	switch (j->t){
		case JType::Num:
		case JType::Str:
			break;
		case JType::Arr:
			for (auto a : *(j->x.a)){
				switch (a->t){
					case JType::Num:
					case JType::Str:
						delete a;
						break;
					default:
						more.push(a);
						break;
				}
			}
			break;
		case JType::Obj:
			for (auto m : *(j->x.m)){
				switch (m.second->t){
					case JType::Num:
					case JType::Str:
						delete m.second;
						break;
					default:
						more.push(m.second);
						break;
				}
			}
			break;
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
	while (!chr.empty()){
		c = chr.get();
		if (c>='0' && c<='9'){
			s += c;
			chr.advance();
		} else {
			break;
		}
	}
	if (c=='.'){
		s += '.';
		chr.advance();
		while (!chr.empty()){
			c = chr.get();
			if (c>='0' && c<='9'){
				s += c;
				chr.advance();
			} else {
				break;
			}
		}
	}
	return std::stof(s);
}
JType mk_key_value(ChunkReader& chr, string& s){
	// Given a ChunkReader at '"' char, modify propname, and return a JType
	s.clear();
	chr.advance();
	chr.capture_until(s,'"');
	if (chr.get()!='"'){
		throw 1; // put an specific exception here.
	}
	if (s.length()==0){
		throw 2; // put an specific exception here. (relating to 0len)
	}
	if (chr.until(':')!=':'){
		throw 3; // put an specific exception here. (relating to missing :)
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
			throw 4; // exception: unexpected char.
			break;
	}
	return JType::Str;
}

void object_handler(stack<Jso*>& stk, ChunkReader& chr){
	string s;
	char c;
	Jso* j;
	while (!chr.empty() && !stk.empty()){
		c = next_symplex(chr);
		if (c=='}'){
			chr.advance();
			stk.pop();
			return;
		} else if (c=='"'){
			switch (mk_key_value(chr,s)){
				case JType::Num:
					stk.top()->key_value(s,get_a_number(chr));
					break;
				case JType::Str:
					stk.top()->key_value(s,chr.capture_until('"'));
					if (chr.get()!='"'){
						throw 1;
					}
					// NEED TO THROW IF " isn't found!
					break;
				case JType::Obj:
					j = new Jso(JType::Obj);
					stk.top()->key_value(s,j);
					stk.emplace(j);
					return;
					break;
				case JType::Arr:
					j = new Jso(JType::Arr);
					stk.top()->key_value(s,j);
					stk.emplace(j);
					return;
					break;
			}
		} else {
			throw 1;
		}
	}
}
void array_handler(stack<Jso*>& stk, ChunkReader& chr){
	
}
