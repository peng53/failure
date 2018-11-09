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
					out << '\t' << j << '\n';
				}
				break;
		}
		return out;
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
};

int main(){
	JSON lv;
	lv.key_value("str","typer");
	lv.key_value("num",3.14);
	lv.key_value("obj",JType::Obj);
	lv.key_value("arr",JType::Arr);
	cout << lv;
	return 0;
}
