#ifndef MY_ST
#define MY_ST
#include "st.h"

ostream& operator<<(ostream& sout,Part& p){
	p.out(sout);
	return sout;
}
void CPart::out(ostream& sout){
	for (unsigned i=r;i>0;--i){
		sout<<*s;
		if (i!=1) sout<<*d;
	}
}
std::mt19937 RNG(time(0));
void RPart::out(ostream& sout){
	std::uniform_int_distribution<unsigned> r(0,c->length()-1);
	for (unsigned i=t;i>0;--i){
		for (unsigned n=0;n<l;++n){
			sout<< (*c)[r(RNG)];
		}
		if (i!=1) sout<<*d;
	}
}
void DPart::out(ostream& sout){
	std::uniform_int_distribution<unsigned> r(0,f.size()-1);
	sout << *(f[r(RNG)]);
}
ostream& operator<<(ostream& out,PartedString &PS){
	for (auto p : PS.parts){
		out<<*p;
	}
	return out;
}
void PartedString::add_lit(const string& s){
	lits.push_back(new string {s});
}
PartedString::~PartedString(){
	for (auto p : parts) delete p;
	for (auto l : lits) delete l;
}
void PartedString::add_part(const string& s){
	lits.push_back(new string {s});
	parts.push_back(new CPart(lits.back()));
}
void PartedString::add_part(const unsigned I){
	parts.push_back(new CPart(lits[I]));
}
void PartedString::add_part(const unsigned I,const unsigned L){
	parts.push_back(new RPart(lits[I],L));
}
void PartedString::add_part(const unsigned I,const unsigned D,const unsigned R){
	if (R!=0){
		parts.push_back(new CPart(lits[I],lits[D],R));
	} else {
		DPart* t = new DPart();
		parts.push_back(t);
		for (unsigned i=I;i<=D;++i){
			t->f.push_back(lits[i]);
		}
	}
}
void PartedString::add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W){
	parts.push_back(new RPart(lits[I],L,lits[D],W));
}

#endif
