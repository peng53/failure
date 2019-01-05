#include "jso.h"
#include <stack>

using std::stack;

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
Jso::Jso(const string& v): Jso(JType::Str){
	*(x.s) = v;
}
Jso::Jso(double v): Jso(JType::Num){
	x.f = v;
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
void Jso::key_value(const string& k,const double v){
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = new Jso(v); }
}
void Jso::key_value(const string& k,JType vt){
	// makes an empty obj or arr
	if (t==JType::Obj && x.m->count(k)==0){ (*(x.m))[k] = new Jso(vt); }
}
void Jso::add_value(const double v){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(v)); }
}
void Jso::add_value(const string& v){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(v)); }
}
void Jso::add_value(const JType vt){
	if (t==JType::Arr){ x.a->emplace_back(new Jso(vt)); }
}
void Jso::add_value(Jso* v){
	if (t==JType::Arr){ x.a->emplace_back(v); }
}
void Jso::set_value(double v){
	if (t==JType::Num){ x.f = v; }
}
void Jso::set_value(const string& v){
	if (t==JType::Str){ *(x.s) = v; }
}
ostream& operator<<(ostream& out,const Jso& J){
	switch (J.t){
		case JType::Num:
			out << J.x.f << 'f';
			break;
		case JType::Str:
			out << "s`" << *(J.x.s) << '`';
			break;
		case JType::Obj:
			out << "{\n";
			for (const auto& j : *(J.x.m)){
				out << j.first << " : ";
				out << *(j.second) << '\n';
			}
			out << "}\n";
			break;
		case JType::Arr:
			out << "[\n";
			for (const auto& j : *(J.x.a)){
				out << *j << '\n';
			}
			out << "]\n";
			break;
	}
	return out;
}
Jso* Jso::key_value(const string& k){
	return ((t==JType::Obj && x.m->count(k)==1) ? (*(x.m))[k] : nullptr);
}
void indent_it(size_t i, ostream& out){
	while (i>1){
		out << ' ';
		--i;
	}
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
	Jso arr_end = Jso("]");
	Jso obj_end = Jso("}");
	string blank_label;
	unsigned ind;
	Jso* j;
	stack<PrintStackNode> stk;
	stk.emplace(label,this,1);
	while (!stk.empty()){
		j = stk.top().obj;
		ind = stk.top().ind;
		indent_it(ind,out);
		if (stk.top().label.length()>0){
			out << stk.top().label << " : ";
		}
		stk.pop();
		switch (j->t){
			case JType::Str:
				if (j==&arr_end){
					out << "]\n";
				}	else if (j==&obj_end){
					out << "}\n";
				} else {
					out << *j << '\n';
				}
				break;
			case JType::Num:
				out << *j << '\n';
				break;
			case JType::Obj:
				stk.emplace(blank_label,&obj_end,ind);
				//indent_it(ind,cout);
				out << "{\n";
				for (auto& kv : *(j->x.m)){
					stk.emplace(kv.first,kv.second,ind+1);
				}
				break;
			case JType::Arr:
				stk.emplace(blank_label,&arr_end,ind);
				//indent_it(ind,cout);
				out << "[\n";
				for (auto& v : *(j->x.a)){
					switch (v->t){
						case JType::Num:
						case JType::Str:
							indent_it(ind+1,out);
							out << *v << '\n';
							break;
						default:
							stk.emplace(blank_label,v,ind+1);
							break;
					}
				}
				break;
		}
	}
}
