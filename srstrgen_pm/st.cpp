#include "st.h"

#include <random>
#include <ctime>

std::mt19937 RNG(time(0));

struct CPart : Part {
	string* s;
	string* d = nullptr;
	unsigned r = 1;
	CPart(string* _s) : s(_s){}
	CPart(string* _s,string* _d,unsigned _r) : s(_s),d(_d),r(_r){}
	void out(ostream& sout){
		for (unsigned i=r;i>0;--i){
			sout<<*s;
			if (i!=1) sout<<*d;
		}
	}
};
struct RPart : Part {
	string* c;
	unsigned l;
	string* d = nullptr;
	unsigned t = 1;
	RPart(string* _c,unsigned _l) : c(_c),l(_l){};
	RPart(string* _c,unsigned _l,string* _d,unsigned _t) : c(_c),l(_l),d(_d),t(_t){};
	void out(ostream& sout){
		std::uniform_int_distribution<unsigned> r(0,c->length()-1);
		for (unsigned i=t;i>0;--i){
			for (unsigned n=0;n<l;++n){
				sout<< (*c)[r(RNG)];
			}
			if (i!=1) sout<<*d;
		}
	}
};
struct DPart : Part {
	vector<string*> f;
	void out(ostream& sout){
		std::uniform_int_distribution<unsigned> r(0,f.size()-1);
		sout << *(f[r(RNG)]);
	}
	DPart(const vector<unique_ptr<string>> &s, const unsigned b, const unsigned e){
		for (unsigned i=b;i<=e; ++i){
			f.push_back(s[i].get());
		}
	}
};

ostream& operator<<(ostream& sout,Part& p){
	p.out(sout);
	return sout;
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
PartedString& PartedString::operator<<(const string& s){
	add_lit(s);
	return *this;
}
void PartedString::add_part(const string& s){
	add_lit(s);
	add_part(lits.size()-1);
}
void PartedString::add_part(const unsigned I){
	parts.emplace_back(make_unique<CPart>(lits[I].get()));
}
PartedString& PartedString::operator<<(const unsigned i){
	add_part(i);
	return *this;
}
void PartedString::add_part(const unsigned I,const unsigned L){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L));
}
void PartedString::add_part(const unsigned I,const unsigned D,const unsigned R){
	if (R!=0){
		parts.emplace_back(make_unique<CPart>(lits[I].get(),lits[D].get(),R));
	} else {
		parts.emplace_back(make_unique<DPart>(lits,I,D));
	}
}
void PartedString::add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L,lits[D].get(),W));
}
size_t PartedString::lits_size(){
	return lits.size();
}
