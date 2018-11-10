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
		case JType::Num:
			cout << "deleted f\n";
			break;
		case JType::Str:
			cout << "deleted s  _" << x.s << '\n';
			delete x.s;
			break;
		case JType::Arr:
			cout << "deleted  a _" << x.a << '\n';
			delete x.a;
			break;
		case JType::Obj:
			cout << "deleted   m_" << x.m << '\n';
			delete x.m;
			break;
	}
	cout << "Bye! " << this << '\n';
}
void Jso::key_value(const string& k,Jso* v){
	// have to make that provide that object ptr yourself
	if (t==JType::Obj && x.m->count(k)==0){
		(*(x.m))[k] = v;
	}
}
void Jso::key_value(const string& k,const string& v){
	if (t==JType::Obj && x.m->count(k)==0){
		(*(x.m))[k] = new Jso(v);
	}
}
void Jso::key_value(const string& k,const float v){
	if (t==JType::Obj && x.m->count(k)==0){
		(*(x.m))[k] = new Jso(v);
	}
}
void Jso::key_value(const string& k,JType vt){
	// makes an empty obj or arr
	if (t==JType::Obj && x.m->count(k)==0){
		(*(x.m))[k] = new Jso(vt);
	}
}
/*
void Jso::add_value(const Jso& v){
	if (t==JType::Arr){
		x.a->emplace_back(new Jso(v));
	}
}
*/
void Jso::add_value(const float v){
	if (t==JType::Arr){
		x.a->emplace_back(new Jso(v));
	}
}
void Jso::add_value(const string& v){
	if (t==JType::Arr){
		x.a->emplace_back(new Jso(v));
	}
}
void Jso::add_value(const JType vt){
	if (t==JType::Arr){
		x.a->emplace_back(new Jso(vt));
	}
}
void Jso::set_value(float v){
	if (t==JType::Num){
		x.f = v;
	}
}
void Jso::set_value(const string& v){
	if (t==JType::Str){
		*(x.s) = v;
	}
}
std::ostream& operator<<(std::ostream& out,const Jso& J){
	switch (J.t){
		case JType::Num:
			out << "number";
			out << J.x.f;
			break;
		case JType::Str:
			out << *(J.x.s);
			break;
		case JType::Obj:
			out << "Object: " << (J.x.m) << "\nKey : Value\n";
			for (auto j : *(J.x.m)){
				out << "  " << j.first << " : ";
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
			}
			break;
		case JType::Arr:
			out << "Array: " << (J.x.a) << "\nValues\n";
			for (auto j : *(J.x.a)){
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
			}
			break;
	}
	return out;
}
void Jso::print_depth(ostream& O, stack<pair<string,Jso*>>& stk){
	// should only be called on ARR or OBJ.
	switch (t){
		case JType::Num:
		case JType::Str:
			break;
		case JType::Arr:
			O << "[\n";
			for (auto A : *(x.a)){
				switch (A->t){
					case JType::Num:
						O << A->x.f << '\n';
						break;
					case JType::Str:
						O << *(A->x.s) << '\n';
						break;
					case JType::Arr:
						stk.emplace("",A);
						break;
					case JType::Obj:
						stk.emplace("",A);
						break;
				}
			}
			O << "]\n";
			break;
		case JType::Obj:
			O << "{\n";
			for (auto A : *(x.m)){
				switch (A.second->t){
					case JType::Num:
						O << A.first << " : " << A.second->x.f << '\n';
						break;
					case JType::Str:
						O << A.first << " : " << *(A.second->x.s) << '\n';
						break;
					case JType::Arr:
						stk.emplace(A.first,A.second);
						break;
					case JType::Obj:
						stk.emplace(A.first,A.second);
						break;
				}
			}
			O << "}\n";
			break;
	}
}
Jso* Jso::key_value(const string& k){
	if (t==JType::Obj && x.m->count(k)==1){
		return (*(x.m))[k];
	}
	return nullptr;
}
void dispose(Jso* j, stack<Jso*>& more,ostream& out){
	out << "Hi! " << j << '\n';
	switch (j->t){
		case JType::Num:
		case JType::Str:
			break;
		case JType::Arr:
			for (auto a : *(j->x.a)){
				switch (a->t){
					case JType::Num:
					case JType::Str:
						//out << "deleted jso_" << a << '\n';
						out << "Hi! " << a << '\n';
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
						//out << "deleted jso_" << m.second << '\n';
						out << "Hi! " << m.second << '\n';
						delete m.second;
						break;
					default:
						more.push(m.second);
						break;
				}
			}
			break;
	}
	//out << "deleted jso_" << j << '\n';
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
	if (o->x.m->count(k)==1){
		return (*(o->x.m))[k];
	}
	return nullptr;
}
void JSON::all_out(ostream& O){
	stack<pair<string,Jso*>> stk;
	stk.emplace("",o);
	string k;
	Jso* v;
	while (!stk.empty()){
		k = stk.top().first;
		v = stk.top().second;
		stk.pop();
		O << k;
		v->print_depth(O,stk);
	}
}

int main(){
	JSON lv; // # 0 obj
	/*
	Jso* my_array = new Jso(JType::Arr); // #1 arr
	cout << my_array << " array made\n";
	my_array->add_value(string("blah")); // #2 str
	my_array->add_value(JType::Obj); // #3 obj
	my_array->add_value(JType::Arr); // #4 arr
	my_array->add_value(10); // # 5 num
	lv.key_value("str","typer"); // #6 str
	lv.key_value("num",3.14); // #7 num
	lv.key_value("obj",JType::Obj); // #8 obj
	lv.key_value("arr",my_array); // #9 arr
	cout << lv << '\n';
	// two addresses will print out,
	// the Jso* and underlying vector.
	cout << my_array << '\n' << *my_array;
	// I don't need to manually delete my_array
	// because lv has it's address
	*/
	lv.key_value("str","test"); // 1
	lv.key_value("num",3.14); // 2
	lv.key_value("arr",JType::Arr); // 3
	lv.key_value("obj",JType::Obj); // 4
	cout << lv << '\n';
	cout << *(lv.key_value("str")) << '\n';
	(lv.key_value("arr"))->add_value(10);
	cout << *(lv.key_value("arr"));
	return 0;
}
