#include <ostream>
#include <string>
#include <random>
#include <ctime>

using std::ostream;
using std::string;

struct Part {
	virtual void out(ostream& sout) = 0;
};
ostream& operator<<(ostream& sout,Part& p);
struct CPart : Part {
	string* s;
	string* d = nullptr;
	unsigned r = 1;
	CPart(string* _s) : s(_s){}
	CPart(string* _s,string* _d,unsigned _r) : s(_s),d(_d),r(_r){}
	void out(ostream& sout);
};
struct RPart : Part {
	string* c;
	unsigned l;
	string* d = nullptr;
	unsigned t = 1;
	RPart(string* _c,unsigned _l) : c(_c),l(_l){};
	RPart(string* _c,unsigned _l,string* _d,unsigned _t) : c(_c),l(_l),d(_d),t(_t){};
	void out(ostream& sout);
};
