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
int main(){
	JSON lv;
	Jso* j = (*lv);
	j->key_value("str","test");
	j->key_value("num",27.2f);
	j->key_value("arr",JType::Arr);
	j->key_value("obj",JType::Obj);
	j = (*lv)->key_value("arr");
	j->add_value(5);
	j->add_value(2);
	j->add_value("horse");
	j = (*lv)->key_value("obj");
	j->key_value("horse","table");
	j->key_value("mouse",3.14f);
	//cout << **lv << "\n------------\n";
	(*lv)->jso_out(cout);
	cout << "\n----------\n";
	//all_out(**lv,cout);
	return 0;
}
