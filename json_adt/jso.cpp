#include "jso.h"
#include <stack>

using std::stack;

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
			out << *(J.x.s);
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

struct PrintStackNode {
	const string& label;
	Jso* obj;
	const unsigned ind;
	PrintStackNode(const string& s,Jso* j,unsigned i):
		label(s), obj(j), ind(i)
		{}
};
void Jso::rprint(ostream& out, const string& label){
	//Jso* arr_end = Str("]");
	//Jso* obj_end = Str("}");
	string blank_label;
	unsigned ind;
	Jso* j;
	stack<PrintStackNode> stk;
	stk.emplace(label,this,1);
	while (!stk.empty()){
		j = stk.top().obj;
		ind = stk.top().ind;
		out << Indentor(ind);
		if (stk.top().label.length()>0){
			out << stk.top().label << " : ";
		}
		stk.pop();
		switch (j->t){
			case JType::Obj:
				//stk.emplace(blank_label,obj_end,ind);
				out << "{\n";
				for (auto& kv : *(j->x.m)){
					stk.emplace(kv.first,kv.second,ind+1);
				}
				break;
			case JType::Arr:
				//stk.emplace(blank_label,arr_end,ind);
				out << "[\n";
				for (auto& v : *(j->x.a)){
					stk.emplace(blank_label,v,ind+1);
				}
				break;
			case JType::Null:
			case JType::True:
			case JType::False:
			case JType::Num:
			case JType::Str:
				out << *j << '\n';
				break;
		}
	}
	//delete obj_end;
	//delete arr_end;
}
const string nonstring;
Jso::operator const string&(){
	if (t==JType::Str){
		return *(x.s);
	} else {
		return nonstring;
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
