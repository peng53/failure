#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>

using std::string;
using std::vector;
using std::map;
using std::queue;
using std::stack;
using std::pair;

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
	void key_value(const string& v,Jso* o){
		if (t==JType::Obj){
			if (x.m->count(v)==0){
				(*(x.m))[v] = o;
			}
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
						D.push(v);
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
	void key_value(const string& v,Jso* n){
		o->key_value(v,n);
	}
};

int main(){
	JSON lv;
	Jso* p = new Jso("typer");
	lv.key_value("first",p);
	delete p;
	return 0;
}