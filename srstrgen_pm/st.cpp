#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <ctime>
#include <list>
#include <vector>

using namespace std;

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

std::mt19937 RNG(time(0));

struct RPart : Part {
	string* c;
	unsigned l;
	string* d = nullptr;
	unsigned t = 1;
	RPart(string* _c,unsigned _l) : c(_c),l(_l){};
	RPart(string* _c,unsigned _l,string* _d,unsigned _t) : c(_c),l(_l),d(_d),t(_t){};
	void out(ostream& sout){
		uniform_int_distribution<unsigned> r(0,c->length()-1);
		for (unsigned i=t;i>0;--i){
			for (unsigned n=0;n<l;++n){
				sout<< (*c)[r(RNG)];
			}
			if (i!=1) sout<<*d;
		}
	}
};

ostream& operator<<(ostream& sout,Part& p){
	p.out(sout);
	return sout;
}

int main(){
	list<Part*> ps;
	vector<string*> lits;
	lits.push_back(new string {"601"});
	lits.push_back(new string {"hello"});
	lits.push_back(new string {"-"});
	ps.push_back(new CPart(lits[1]));
	ps.push_back(new CPart(lits[2]));
	ps.push_back(new RPart(lits[0],4,lits[2],3));
	for (auto p: ps){
		cout << *p;
	}
	cout << '\n';
	for (auto l : lits){
		delete l;
	}
	for (auto p : ps){
		delete p;
	}
	return 0;

}
