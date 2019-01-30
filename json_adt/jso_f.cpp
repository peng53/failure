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
		case JType::Str:
		case JType::Num:
			break;
		default:
			if (j==JSON::Single(j->t)){
				return;
			}
			break;
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
	//rhs.o->rprint(out,"Object");
	tprint(out,rhs.o);
	return out;
}

Jso* JSON::Str(const string& v){
	Jso* j = new Jso(JType::Str);
	j->x.s = new string(v);
	return j;
}
Jso* JSON::Num(const double v){
	Jso* j = new Jso(JType::Num);
	j->x.f = v;
	return j;
}
Jso* JSON::Map(){
	Jso* j = new Jso(JType::Obj);
	j->x.m = new map<string,Jso*>;
	return j;
}
Jso* JSON::Arr(){
	Jso* j = new Jso(JType::Arr);
	j->x.a = new vector<Jso*>;
	return j;
}
Jso* JSON::Single(JType t){
	switch (t){
		case JType::Null: return &Null;
		case JType::True: return &True;
		case JType::False: return &False;
		default:
			return nullptr;
	}
}
Jso JSON::Null = Jso(JType::Null);
Jso JSON::True = Jso(JType::True);
Jso JSON::False = Jso(JType::False);
