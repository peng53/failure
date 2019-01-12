#include "jso_f.h"
#include <stack>

using std::stack;
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
		case JType::Null:
		case JType::True:
		case JType::False:
			return;
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
		for (auto& v : *(r->x.m)){
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
				case JType::Null:
					(*(l->x.m))[v.first] = &(Jso::JSO_NULL);
					break;
				case JType::True:
					(*(l->x.m))[v.first] = &(Jso::JSO_TRUE);
					break;
				case JType::False:
					(*(l->x.m))[v.first] = &(Jso::JSO_FALSE);
					break;
			}
		}
	}
}
Jso* JSON::operator*(){
	return o;
}

ostream& operator<<(ostream& out,const JSON& rhs){
	rhs.o->rprint(out,"Object");
	return out;
}
