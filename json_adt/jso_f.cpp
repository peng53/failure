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
	explicit Jso(string v): t(JType::Str){
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
	JSON(const JSON& lhs): JSON(){
		Jso* s = lhs.o;
		Jso* t;
		stack<Jso*> stk;
		stk.push(s);
		while (!stk.empty()){
			s = stk.top();
			stk.pop();
			for (auto v : *(s->x.m)){
				switch (v.second->t){
					case JType::Str:
						(*(s->x.m))[v.first] = new Jso(*(v.second->x.s));
						// Assign to map a new string:string
						break;
					case JType::Num:
						(*(s->x.m))[v.first] = new Jso(v.second->x.f);
						// x.f is just a float, so no dereferencing needed.
						break;
					case JType::Obj:
						t = new Jso(JType::Obj);
						(*(s->x.m))[v.first] = t;
						stk.push(t);
						break;
					case JType::Arr:
						break;
				}
			}
		}
	}
};
