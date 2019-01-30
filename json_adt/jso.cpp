#include "jso.h"
#include <stack>

using std::stack;

Jso::Jso(JType jt): t(jt){}

Jso::~Jso(){
	// Only deletes its members; doesn't go further.
	SetVoid();
}
void Jso::Append(const string& k,Jso* v){
	// have to make that provide that object ptr yourself
	if (t==JType::Obj &&
		v!=nullptr &&
		v!=this &&
		x.m->count(k)==0
	){
		x.m->emplace(k,v);
	}
}
void Jso::Append(Jso* v){
	if (t==JType::Arr &&
		v!=nullptr &&
		v!=this
	){
		x.a->emplace_back(v);
	}
}

ostream& operator<<(ostream& out,const Jso& J){
	switch (J.t){
		case JType::Num:
			out << J.x.f;
			break;
		case JType::Str:
			out << '"' << *(J.x.s) << '"';
			break;
		case JType::Obj:
			out << "Object{" << J.x.m->size() << '}';
			break;
		case JType::Arr:
			out << "Array[" << J.x.a->size() << ']';
			break;
		case JType::Null:
			out << "null";
			break;
		case JType::True:
			out << "true";
			break;
		case JType::False:
			out << "false";
			break;
	}
	return out;
}
Jso* Jso::key_value(const string& k){
	return ((t==JType::Obj && x.m->count(k)==1) ? (*(x.m))[k] : nullptr);
}
Jso* Jso::operator[](const string& k){
	return key_value(k);
}

struct Indentor {
	const unsigned spaces;
	Indentor(unsigned n): spaces(n){}
};
ostream& operator<<(ostream& out,const Indentor& ind){
	for (unsigned i=ind.spaces;i>1;--i){
		out << ' ';
	}
	return out;
}

struct Pnode {
	Jso* o;
	const unsigned i;
	const string& s;
	Pnode(Jso* j,unsigned ind,const string& label=""):
		o(j), i(ind), s(label){}
};

void tprint(ostream& out, Jso* root){
	stack<JType> last;
	stack<Pnode> to_print;
	last.emplace(root->t);
	unsigned layer = 1;
	to_print.emplace(root,layer);
	Jso* current;
	while (!to_print.empty()){
		current = to_print.top().o;
		if (layer>to_print.top().i){
			out << Indentor(to_print.top().i);
			if (last.top()==JType::Obj){
				out << "}\n";
			} else {
				out << "]\n";
			}
			last.pop();
		}
		layer = to_print.top().i;
		out << Indentor(layer);
		if (to_print.top().s!=""){
			out << '"' << to_print.top().s << "\" : ";
		}
		to_print.pop();
		switch (current->t){
			case JType::Null:
			case JType::True:
			case JType::False:
			case JType::Num:
			case JType::Str:
				out << *current;
				if (!to_print.empty() && to_print.top().i==layer){
					out << ',';
				}
				out << '\n';
				break;
			case JType::Arr:
				out << "[\n";
				last.emplace(JType::Arr);
				for (auto& j : *(current->x.a)){
					to_print.emplace(j,layer+1);
				}
				break;
			case JType::Obj:
				out << "{\n";
				last.emplace(JType::Obj);
				for (const auto& kv : *(current->x.m)){
					to_print.emplace(kv.second,layer+1,kv.first);
				}
				break;
		}
	}
	if (!last.empty()){
		last.pop();
		if (last.top()==JType::Obj){
			out << "}\n";
		} else {
			out << "]\n";
		}
	}
}

void Jso::Get(string **outptr){
	(*outptr) = ((t==JType::Str) ? x.s : nullptr);
}
void Jso::Get(double **outptr){
	(*outptr) = ((t==JType::Num) ? &x.f : nullptr);
}
void Jso::Get(map<string,Jso*> **outptr){
	(*outptr) = ((t==JType::Obj) ? x.m : nullptr);
}
void Jso::Get(vector<Jso*> **outptr){
	(*outptr) = ((t==JType::Arr) ? x.a : nullptr);
}
void Jso::SetVoid(double v){
	// All ptrs are lost, be sure to get them before hand!
	switch (t){
		case JType::Null:
		case JType::True:
		case JType::False:
			// Should NOT be modifying these.
			return;
		case JType::Arr:
			delete x.a;
			break;
		case JType::Str:
			delete x.s;
			break;
		case JType::Obj:
			delete x.m;
			break;
		default:
			break;
	}
	t = JType::Num;
	x.f = v;
}
void Jso::Append(const pair<string, Jso*>& kv){
	Append(kv.first,kv.second);
}
string* Jso::Str(){
	// returns the string ptr of this.
	// if this isn't a str, return nullptr.
	return ((t==JType::Str) ? x.s : nullptr);
}
