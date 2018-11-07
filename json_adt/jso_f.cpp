#include <string>
#include <vector>
#include <map>
#include <queue>

using std::string;
using std::vector;
using std::map;
using std::queue;

enum class JType { Obj, Arr, Str, Num};

class Jso {
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
	void destroy(){
		queue<Jso*> D;
		Jso* d = this;
		D.push(d);
		while (!D.empty()){
			d = D.front();
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

};
