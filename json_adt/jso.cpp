#include "jso.h"

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
			for (auto j : *(J.x.m)){
				out << j.first << " : ";
				out << *(j.second) << '\n';
			}
			out << "}\n";
			break;
		case JType::Arr:
			out << "[\n";
			for (auto j : *(J.x.a)){
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

struct JsoItr {
	JType t;
	union V {
		pair<map<string,Jso*>::iterator,map<string,Jso*>::iterator> o;
		pair<vector<Jso*>::iterator,vector<Jso*>::iterator> a;
		V(map<string,Jso*>::iterator s,map<string,Jso*>::iterator e):
			o(make_pair(s,e)) {}
		V(vector<Jso*>::iterator s,vector<Jso*>::iterator e):
			a(make_pair(s,e)) {}
	};
	V v;
	JsoItr(map<string,Jso*>::iterator s,map<string,Jso*>::iterator e):
		t(JType::Obj),
		v(s,e) {}
	JsoItr(vector<Jso*>::iterator s,vector<Jso*>::iterator e):
		t(JType::Arr),
		v(s,e) {}
	bool eoo(){
		switch (t){
			case JType::Arr: return v.a.first == v.a.second; break;
			case JType::Obj: return v.o.first == v.o.second; break;
			default: return 1; break;
		}
		return 1;
	}
	JsoItr& operator++(){
		switch (t){
			case JType::Arr: ++v.a.first; break;
			case JType::Obj: ++v.o.first; break;
			default: break;
		}
		return *this;
	}
};

void indent_it(size_t i, ostream& out){
	while (i>1){
		out << ' ';
		--i;
	}
}

void adder(JsoItr& j,stack<JsoItr>& stk,ostream& out,size_t& sp){
	// prints if num or str, otherwise add to stack then return.
	switch (j.t){
		default: break;
		case JType::Arr:
			for (auto i=j.v.a.first, e=j.v.a.second; i!=e; ++i){
				switch ((*i)->t){
					case JType::Arr:
						out << "[\n";
						stk.emplace(++i,e);
						--i;
						sp++;
						stk.emplace((*i)->x.a->begin(),(*i)->x.a->end());
						return;
						break;
					case JType::Obj:
						out << "{\n";
						stk.emplace(++i,e);
						--i;
						sp++;
						stk.emplace((*i)->x.m->begin(),(*i)->x.m->end());
						return;
						break;
					default:
						indent_it(sp,out);
						out << **i << ",\n";
						break;
				}
			}
			indent_it(sp,out);
			sp--;
			out << "]\n";
			break;
		case JType::Obj:
			for (auto i=j.v.o.first, e=j.v.o.second; i!=e; ++i){
				indent_it(sp,out);
				out << i->first << " := ";
				switch ((*i).second->t){
					case JType::Arr:
						out << "[\n";
						stk.emplace(++i,e);
						--i;
						sp++;
						stk.emplace((*i).second->x.a->begin(),(*i).second->x.a->end());
						return;
						break;
					case JType::Obj:
						out << "{\n";
						stk.emplace(++i,e);
						--i;
						sp++;
						stk.emplace((*i).second->x.m->begin(),(*i).second->x.m->end());
						return;
						break;
					default:
						out << *((*i).second) << '\n';
						break;
				}
			}
			indent_it(sp,out);
			sp--;
			out << "}\n";
			break;
	}
}

void Jso::jso_out(ostream& out){
	stack<JsoItr> stk;
	switch (t){
		case JType::Str:
		case JType::Num:
			out << *this;
			break;
		case JType::Arr:
			stk.emplace(x.a->begin(),x.a->end());
			out << "[\n";
			break;
		case JType::Obj:
			stk.emplace(x.m->begin(),x.m->end());
			out << "{\n";
			break;
	}
	size_t i = 1;
	while (!stk.empty()){
		JsoItr itt = stk.top();
		stk.pop();
		adder(itt,stk,out,i);
	}
}

enum class ObjOrArrE { OBJ,ARR};

struct ObjOrArr {
	union Itr {
		struct {
			vector<Jso*>::iterator I;
			vector<Jso*>::iterator E;
		} A;
		struct {
			map<string,Jso*>::iterator I;
			map<string,Jso*>::iterator E;
		} O;
		Itr(vector<Jso*>* v){
			A.I = v->begin();
			A.E = v->end();
		}
		Itr(map<string,Jso*>* m){
			O.I = m->begin();
			O.E = m->end();
		}
	};
	ObjOrArrE t;
	Itr d;
	ObjOrArr(vector<Jso*>* j):
		t(ObjOrArrE::ARR),
		d(j){}
	ObjOrArr(map<string,Jso*>* j):
		t(ObjOrArrE::OBJ),
		d(j){}
	ObjOrArr& operator++(){
		switch (t){
			case ObjOrArrE::OBJ: ++d.O.I; break;
			case ObjOrArrE::ARR: ++d.A.I; break;
		}
		return *this;
	}
	bool END(){
		return ((t==ObjOrArrE::OBJ) ? d.O.I==d.O.E : d.A.I==d.A.E);
	}
	Jso* operator*(){
		return ((t==ObjOrArrE::OBJ) ? d.O.I->second : *(d.A.I));
	}
	const string& first(){
		return d.O.I->first;
	}
};

void adder2(stack<ObjOrArr>& stk,ostream& out){
	if (stk.empty()) return;
	ObjOrArr& T = stk.top();
	if (T.END()) return;
	switch (T.t){
		case ObjOrArrE::ARR:
			while (!T.END()){
				switch ((*T)->t){
					case JType::Arr:
						out << "[\n";
						stk.emplace((*T)->x.a);
						++T;
						return;
						break;
					case JType::Obj:
						out << "{\n";
						stk.emplace((*T)->x.m);
						++T;
						return;
						break;
					default: out << **T << '\n'; break;
				}
				++T;
			}
			out << "]\n";
			if (!stk.empty()) stk.pop();
			break;
		case ObjOrArrE::OBJ:
			while (!T.END()){
				out << T.first() << " := ";
				switch ((*T)->t){
					case JType::Arr:
						out << "[\n";
						stk.emplace((*T)->x.a);
						++T;
						return;
						break;
					case JType::Obj:
						out << "{\n";
						stk.emplace((*T)->x.m);
						++T;
						return;
						break;
					default: out << **T << '\n'; break;
				}
				++T;
			}
			out << "}\n";
			if (!stk.empty()) stk.pop();
			break;
	}
}
void all_out(Jso j,ostream& out){
	stack<ObjOrArr> stk;
	stk.emplace(j.x.m);
	while (!stk.empty()){
		adder2(stk,out);
	}
}