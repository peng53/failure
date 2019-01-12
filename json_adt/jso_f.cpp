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
Jso* JSON::operator*(){
	return o;
}

ostream& operator<<(ostream& out,const JSON& rhs){
	rhs.o->rprint(out,"Object");
	return out;
}
