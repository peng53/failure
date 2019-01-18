#include "jso_f.h"
#include <stack>

using std::stack;

static void dispose(Jso* j, stack<Jso*>& more){
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
			std::cout << "Singleton: " << j << '\n';
			return;
		default: break;
	}
	delete j;
}

JSON::JSON(): o(Map()){
}

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

Jso* JSON::Str(const string& v){
	Jso* j = new Jso();
	j->t = JType::Str;
	j->x.s = new string(v);
	return j;
}
Jso* JSON::Num(const double v){
	Jso* j = new Jso();
	j->t = JType::Num;
	j->x.f = v;
	return j;
}
Jso* JSON::Map(){
	Jso* j = new Jso();
	j->t = JType::Obj;
	j->x.m = new map<string,Jso*>;
	return j;
}
Jso* JSON::Arr(){
	Jso* j = new Jso();
	j->t = JType::Arr;
	j->x.a = new vector<Jso*>;
	return j;
}
Jso* JSON::Null(){
	static Jso j = Jso();
	j.t = JType::Null;
	Jso* j_ptr = &j;
	return j_ptr;
}
Jso* JSON::True(){
	static Jso j = Jso();
	j.t = JType::True;
	Jso* j_ptr = &j;
	return j_ptr;
}
Jso* JSON::False(){
	static Jso j = Jso();
	j.t = JType::False;
	Jso* j_ptr = &j;
	return j_ptr;
}
