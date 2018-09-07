#include "ste.h"
#include <random>
#include <vector>
#include <list>
#include <memory>
#include <ctime>
using std::unique_ptr;
using std::vector;

// http://stackoverflow.com/questions/24609271/errormake-unique-is-not-a-member-of-std
template<typename T, typename... Args>
static std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
static std::mt19937 RNG(time(0));
struct Part {
	virtual void out(ostream& sout) = 0;
};
struct CPart : Part {
	string* s;
	string* d = nullptr;
	size_t r = 1;
	CPart(string* _s) : s(_s){}
	CPart(string* _s,string* _d,size_t _r) : s(_s),d(_d),r(_r){}
	void out(ostream& sout){
		for (size_t i=r;i>0;--i){
			sout<<*s;
			if (i!=1) sout<<*d;
		}
	}
};
struct RPart : Part {
	string* c;
	size_t l;
	string* d = nullptr;
	size_t t = 1;
	RPart(string* _c,size_t _l) : c(_c),l(_l){};
	RPart(string* _c,size_t _l,string* _d,size_t _t) : c(_c),l(_l),d(_d),t(_t){};
	void out(ostream& sout){
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
	std::vector<string*> f;
	void out(ostream& sout){
		std::uniform_int_distribution<size_t> r(0,f.size()-1);
		sout << *(f[r(RNG)]);
	}
	DPart(const std::vector<unique_ptr<string>> &s, const size_t b, const size_t e){
		for (size_t i=b;i<=e; ++i){
			f.push_back(s[i].get());
		}
	}
};
struct PartedString::IMPL {
	std::list<unique_ptr<Part>> parts;
	std::vector<unique_ptr<string>> lits;
	void add_lit(const string& s){
		lits.push_back(make_unique<string>(s));
	}
};
PartedString::PartedString(){
	M = new IMPL();
}
PartedString::~PartedString(){
	delete M;
}
size_t PartedString::lits_size() const {
	return M->lits.size();
}
ostream& operator<<(ostream& sout,const PartedString &PS){
	for (auto const &p : PS.M->parts){
		(*p.get()).out(sout);
	}
	return sout;
}
PartedString& PartedString::operator<<(const string& s){
	M->add_lit(s);
	return *this;
}
void PartedString::add_part(const string& s){
	M->add_lit(s);
	M->parts.emplace_back(make_unique<CPart>(M->lits.back().get()));
}
void PartedString::add_part(const size_t I){
	M->parts.emplace_back(make_unique<CPart>(M->lits[I].get()));
}
PartedString& PartedString::operator<<(const size_t i){
	add_part(i);
	return *this;
}
void PartedString::add_part(const size_t I,const size_t L){
	M->parts.emplace_back(make_unique<RPart>(M->lits[I].get(),L));
}
void PartedString::add_part(const size_t I,const size_t D,const size_t R){
	if (R!=0){
		M->parts.emplace_back(make_unique<CPart>(M->lits[I].get(),M->lits[D].get(),R));
	} else {
		M->parts.emplace_back(make_unique<DPart>(M->lits,I,D));
	}
}
void PartedString::add_part(const size_t I,const size_t L,const size_t D,const size_t W){
	M->parts.emplace_back(make_unique<RPart>(M->lits[I].get(),L,M->lits[D].get(),W));
}
