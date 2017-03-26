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
	for (auto const &p : PS.parts){
		out<<*p.get();
	}
	return out;
}
// http://stackoverflow.com/questions/24609271/errormake-unique-is-not-a-member-of-std
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
void PartedString::add_lit(const string& s){
	lits.push_back(make_unique<string>(s));
}
PartedString& operator<<(PartedString& p, const string& s){
	p.add_lit(s);
	return p;
}
void PartedString::add_part(const string& s){
	add_lit(s);
	parts.emplace_back(make_unique<CPart>(lits.back().get()));
}
void PartedString::add_part(const unsigned I){
	parts.emplace_back(make_unique<CPart>(lits[I].get()));
}
void PartedString::add_part(const unsigned I,const unsigned L){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L));
}
void PartedString::add_part(const unsigned I,const unsigned D,const unsigned R){
	if (R!=0){
		parts.emplace_back(make_unique<CPart>(lits[I].get(),lits[D].get(),R));
	} else {
		unique_ptr<DPart> t = make_unique<DPart>();
		for (unsigned i=I;i<=D;++i){
			t.get()->f.push_back(lits[i].get());
		}
		parts.push_back(move(t));
	}
}
void PartedString::add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L,lits[D].get(),W));
}
#endif
