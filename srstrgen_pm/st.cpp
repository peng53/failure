#include "st.h"
#include <random>
#include <ctime>

std::mt19937 RNG(time(0));

struct CPart : Part {
	const string *s, *d;
	const size_t r;
	CPart(const string* _s,const string* _d=nullptr,const size_t _r=1) : s(_s),d(_d),r(_r){}
	void out(ostream& sout) const {
		for (size_t i=r;i>0;--i){
			sout<<*s;
			if (i!=1) sout<<*d;
		}
	}
};
struct RPart : Part {
	const string *c, *d;
	const size_t l, t;
	RPart(const string* _c,size_t _l,const string* _d=nullptr,const size_t _t=1) :
		c(_c),l(_l),d(_d),t(_t){}
	void out(ostream& sout) const {
		std::uniform_int_distribution<size_t> r(0,c->length()-1);
		for (size_t i=t;i>0;--i){
			for (size_t n=0;n<l;++n){
				sout<< (*c)[r(RNG)];
			}
			if (i!=1) sout<<*d;
		}
	}
};
struct DPart : Part {
	vector<string*> f;
	void out(ostream& sout) const {
		std::uniform_int_distribution<size_t> r(0,f.size()-1);
		sout << *(f[r(RNG)]);
	}
	DPart(const vector<unique_ptr<string>> &s, const size_t b, const size_t e){
		for (size_t i=b;i<=e; ++i){
			f.push_back(s[i].get());
		}
	}
};

ostream& operator<<(ostream& out,const PartedString &PS){
	for (auto const &p : PS.parts){
		p.get()->out(out);
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
void PartedString::add_part(const size_t I){
	parts.emplace_back(make_unique<CPart>(lits[I].get()));
}
PartedString& PartedString::operator<<(const size_t i){
	add_part(i);
	return *this;
}
void PartedString::add_part(const size_t I,const size_t L){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L));
}
void PartedString::add_part(const size_t I,const size_t D,const size_t R){
	if (R!=0){
		parts.emplace_back(make_unique<CPart>(lits[I].get(),lits[D].get(),R));
	} else {
		parts.emplace_back(make_unique<DPart>(lits,I,D));
	}
}
void PartedString::add_part(const size_t I,const size_t L,const size_t D,const size_t W){
	parts.emplace_back(make_unique<RPart>(lits[I].get(),L,lits[D].get(),W));
}
size_t PartedString::lits_size() const {
	return lits.size();
}
