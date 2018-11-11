#include "jso_f.h"
#include <queue>

using std::cout;
using std::queue;

Jso::Jso(float v): t(JType::Num){
	x.f = v;
}
Jso::Jso(const string& v): t(JType::Str){
	x.s = new string(v);
}
Jso::Jso(JType j): t(j){
	switch (j){
		case JType::Obj:
			x.m = new map<string,Jso*>;
			break;
		case JType::Arr:
			x.a = new vector<Jso*>;
			break;
		case JType::Num:
			x.f = 0;
			break;
		case JType::Str:
			x.s = new string;
			break;
	}
}
Jso::~Jso(){
	// Only deletes its members; doesn't go further.
	switch (t){
		case JType::Num: break;
		case JType::Str: delete x.s; break;
		case JType::Arr: delete x.a; break;
		case JType::Obj: delete x.m; break;
	}
}
void Jso::key_value(const string& k,Jso* v){
	// have to make that provide that object ptr yourself
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = v; }
}
void Jso::key_value(const string& k,const string& v){
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = new Jso(v); }
}
void Jso::key_value(const string& k,const float v){
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = new Jso(v); }
}
void Jso::key_value(const string& k,JType vt){
	// makes an empty obj or arr
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = new Jso(vt); }
}
void Jso::add_value(const float v){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(v)); }
}
void Jso::add_value(const string& v){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(v)); }
}
void Jso::add_value(const JType vt){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(vt)); }
}
void Jso::set_value(float v){
	if (t==JType::Num){ x.f = v; }
}
void Jso::set_value(const string& v){
	if (t==JType::Str){ *(x.s) = v; }
}
std::ostream& operator<<(std::ostream& out,const Jso& J){
	switch (J.t){
		case JType::Num:
			out << J.x.f;
			break;
		case JType::Str:
			out << *(J.x.s);
			break;
		case JType::Obj:
			//out << "Object: " << (J.x.m) << "\nKey : Value\n";
			out << "{\n";
			for (auto j : *(J.x.m)){
				out << j.first << " : ";
				out << *(j.second) << '\n';
				/*
				switch (j.second->t){
					case JType::Str:
						out << *(j.second->x.s);
						break;
					case JType::Num:
						out << j.second->x.f;
						break;
					case JType::Obj:
						out << "OBJ_" << j.second;
						break;
					case JType::Arr:
						out << "ARR_" << j.second;
						break;
				}
				out << '\n';
				*/
			}
			out << "}\n";
			break;
		case JType::Arr:
			//out << "Array: " << (J.x.a) << "\nValues\n";
			out << "[\n";			
			for (auto j : *(J.x.a)){
				out << *j << '\n';
				/*
				switch (j->t){
					case JType::Str:
						out << "  " << *(j->x.s);
						break;
					case JType::Num:
						out << "  " << j->x.f;
						break;
					case JType::Obj:
						out << "  " << "OBJ_" << j;
						break;
					case JType::Arr:
						out << "  " << "ARR_" << j;
						break;
				}
				out << '\n';
			*/
			}
			out << "]\n";
			break;
	}
	return out;
}
Jso* Jso::key_value(const string& k){
	return ((t==JType::Obj && x.m->count(k)==1) ? (*(x.m))[k] : nullptr);
}
void dispose(Jso* j, stack<Jso*>& more,ostream& out){
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
	D.push(d);
	while (!D.empty()){
		d = D.top();
		D.pop();
		dispose(d,D,cout);
	}
}
JSON::JSON(const JSON& rhs): JSON(){
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
					break;
			}
		}
	}
}
ostream& operator<<(ostream& O,const JSON& J){
	O << *(J.o);
	return O;
}
void JSON::key_value(const string& k,Jso* v){
	o->key_value(k,v);
}
void JSON::key_value(const string& k,const string& v){
	o->key_value(k,v);
}
void JSON::key_value(const string& k,const float v){
	o->key_value(k,v);
}
void JSON::key_value(const string& k,JType vt){
	o->key_value(k,vt);
}
Jso* JSON::key_value(const string& k){
	return ((o->x.m->count(k)==1) ? (*(o->x.m))[k] : nullptr);
}

int main(){
	JSON lv; // # 0 obj
	lv.key_value("str","test"); // 1
	lv.key_value("num",3.14); // 2
	lv.key_value("arr",JType::Arr); // 3
	lv.key_value("obj",JType::Obj); // 4
	//cout << *(lv.key_value("str")) << '\n';
	(lv.key_value("arr"))->add_value(10);
	(lv.key_value("arr"))->add_value(27);
	(lv.key_value("arr"))->add_value("horse");
	(lv.key_value("obj"))->key_value("horse","table");
	(lv.key_value("obj"))->key_value("mouse",3.14);
	//cout << *(lv.key_value("arr"));
	cout << lv << '\n';
	return 0;
}
