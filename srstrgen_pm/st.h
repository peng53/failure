#include <ostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <list>
#include <memory>

using std::ostream;
using std::string;
using std::vector;
using std::list;
using std::unique_ptr;
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
struct DPart : Part {
	vector<string*> f;
	void out(ostream& sout);
};
class PartedString {
	private:
		vector<unique_ptr<string>> lits;
		list<unique_ptr<Part>> parts;
		friend ostream& operator<<(ostream& out,PartedString &PS);
	public:
		void add_lit(const string& s);
		void add_part(const string& s);
		void add_part(const unsigned I);
		void add_part(const unsigned I,const unsigned L);
		void add_part(const unsigned I,const unsigned D,const unsigned R);
		void add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W);
};
