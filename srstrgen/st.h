#include <ostream>
#include <string>
#include <list>
#include <vector>
#include <random>

using std::string;
using std::ostream;
//using std::cout;

struct ST {
	unsigned L = 0, W = 1;
	string * I = nullptr;
	string * D = nullptr;
	ST(string* s) : I(s){}
	ST(string* s,unsigned c) : L(c),I(s){}
	ST(string* s,string* d,unsigned r) : W(r),I(s),D(d){}
	ST(string* s,unsigned c,string* d,unsigned r) : L(c),W(r),I(s),D(d){}
};
ostream& operator<<(ostream& out,ST &P);
class PartedString {
	private:
		std::vector<string*> literals;
		std::list<ST> parts;
		friend ostream& operator<<(ostream& out,PartedString &PS);
	public:
		size_t lits_size() const{
			return literals.size();
		}
		void add_literal(const string &s);
		PartedString& operator<<(const string &s);
		void add_part(const string &s);
		void add_part(const unsigned I);
		void add_part(const unsigned I,const unsigned L);
		void add_part(const unsigned I,const unsigned D,const unsigned R);
		void add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W);
		~PartedString(){
			for(auto s : literals){
				/*cout<< "deleting " << s << '\n';*/ delete s;
			}
		}
};
ostream& operator<<(ostream& out,PartedString &PS);
