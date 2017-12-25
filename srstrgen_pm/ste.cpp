#include "ste.h"
#include <random>
#include <vector>
#include <list>
#include <memory>

using std::unique_ptr;

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
	std::vector<string*> f;
	void out(ostream& sout){
		std::uniform_int_distribution<unsigned> r(0,f.size()-1);
		sout << *(f[r(RNG)]);
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
unsigned PartedString::lits_size(){
	return M->lits.size();
}
ostream& operator<<(ostream& sout,PartedString &PS){
	for (auto const &p : PS.M->parts){
		(*p.get()).out(sout);
	}
	return sout;
}
PartedString& operator<<(PartedString& p, const string& s){
	p.M->add_lit(s);
	return p;
}
void PartedString::add_part(const string& s){
	M->add_lit(s);
	M->parts.emplace_back(make_unique<CPart>(M->lits.back().get()));
}
void PartedString::add_part(const unsigned I){
	M->parts.emplace_back(make_unique<CPart>(M->lits[I].get()));
}
PartedString& operator<<(PartedString& p, const unsigned i){
	p.add_part(i);
	return p;
}
void PartedString::add_part(const unsigned I,const unsigned L){
	M->parts.emplace_back(make_unique<RPart>(M->lits[I].get(),L));
}
void PartedString::add_part(const unsigned I,const unsigned D,const unsigned R){
	if (R!=0){
		M->parts.emplace_back(make_unique<CPart>(M->lits[I].get(),M->lits[D].get(),R));
	} else {
		unique_ptr<DPart> t = make_unique<DPart>();
		for (unsigned i=I;i<=D;++i){
			t.get()->f.push_back(M->lits[i].get());
		}
		M->parts.push_back(move(t));
	}
}
void PartedString::add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W){
	M->parts.emplace_back(make_unique<RPart>(M->lits[I].get(),L,M->lits[D].get(),W));
}
