#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <iostream>

using std::string;
using std::vector;
using std::map;
using std::queue;
using std::stack;
using std::pair;
using std::cout;
using std::ostream;

enum class JType { Obj, Arr, Str, Num};

struct Jso {
	JType t;
	union {
		string* s;
		float f;
		vector<Jso*>* a;
		map<string,Jso*>* m;
	} x;
	public:
	explicit Jso(float v): t(JType::Num){
		x.f = v;
	}
	/*explicit Jso(string v): t(JType::Str){
		x.s = new string(v);
	}*/
	explicit Jso(const string& v): t(JType::Str){
		x.s = new string(v);
	}
	explicit Jso(JType j): t(j){
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
	void key_value(const string& k,Jso* v){
		// have to make that provide that object ptr yourself
		if (t==JType::Obj){
			if (x.m->count(k)==0){
				(*(x.m))[k] = v;
			}
		}
	}
	void key_value(const string& k,const string& v){
		if (t==JType::Obj){
			if (x.m->count(k)==0){
				(*(x.m))[k] = new Jso(v);
			}
		}
	}
	void key_value(const string& k,const float v){
		if (t==JType::Obj){
			if (x.m->count(k)==0){
				(*(x.m))[k] = new Jso(v);
			}
		}
	}
	void key_value(const string& k,JType vt){
		// makes an empty obj or arr
		if (t==JType::Obj){
			if (x.m->count(k)==0){
				(*(x.m))[k] = new Jso(vt);
			}
		}
	}
	void add_value(const string& v){
		if (t==JType::Arr){
			x.a->emplace_back(new Jso(v));
		}
	}
	void add_value(const float v){
		if (t==JType::Arr){
			x.a->emplace_back(new Jso(v));
		}
	}
	void add_value(JType vt){
		if (t==JType::Arr){
			x.a->emplace_back(new Jso(vt));
		}
	}
	void add_value(Jso* v){
		if (t==JType::Arr){
			x.a->emplace_back(v);
		}
	}
	void set_value(float v){
		if (t==JType::Num){
			x.f = v;
		}
	}
	void set_value(const string& v){
		if (t==JType::Str){
			*(x.s) = v;
		}
	}
	friend std::ostream& operator<<(std::ostream& out,const Jso J){
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
	void print_depth(ostream& O, stack<pair<string,Jso*>>& stk){
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
};

class JSON {
	Jso* o;
	public:
	JSON(): o(new Jso(JType::Obj)){}
	~JSON(){
		stack<Jso*> D;
		Jso* d = o;
		D.push(d);
		while (!D.empty()){
			d = D.top();
			cout << "Del " << d << '\n';
			D.pop();
			switch (d->t){
				case JType::Num:
					break;
				case JType::Str:
					delete d->x.s;
					break;
				case JType::Obj:
					for (auto v : *(d->x.m)){
						D.push(v.second);
					}
					delete d->x.m;
					break;
				case JType::Arr:
					for (auto v : *(d->x.a)){
						if (v->t==JType::Num){
							delete v;
						} else {
							D.push(v);
						}
					}
					delete d->x.a;
					break;
			}
			delete d;
		}
	}
	JSON(const JSON& rhs): JSON(){
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
	friend ostream& operator<<(ostream& O,const JSON& J){
		O << *(J.o);
		return O;
	}
	void key_value(const string& k,Jso* v){
		o->key_value(k,v);
	}
	void key_value(const string& k,const string& v){
		o->key_value(k,v);
	}
	void key_value(const string& k,const float v){
		o->key_value(k,v);
	}
	void key_value(const string& k,JType vt){
		o->key_value(k,vt);
	}
	void all_out(ostream& O){
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
};

int main(){
	JSON lv;
	Jso* my_array = new Jso(JType::Arr);
	cout << my_array << " array made\n";
	my_array->add_value("hungry");
	lv.key_value("str","typer");
	lv.key_value("num",3.14);
	lv.key_value("obj",JType::Obj);
	lv.key_value("arr",my_array);
	cout << lv << '\n';
	// two addresses will print out,
	// the Jso* and underlying vector.
	cout << my_array << '\n' << *my_array;
	// I don't need to manually delete my_array
	// because lv has it's address
	lv.all_out(cout);
	return 0;
}
