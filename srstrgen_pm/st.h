#ifndef MY_ST
#define MY_ST
#include <ostream>
#include <string>
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
class PartedString {
	private:
		vector<unique_ptr<string>> lits;
		list<unique_ptr<Part>> parts;
		friend ostream& operator<<(ostream& out,PartedString &PS);
		void add_lit(const string& s);
	public:
		PartedString(){}
		~PartedString(){}
		size_t lits_size();
		void add_part(const string& s);
		void add_part(const unsigned I);
		void add_part(const unsigned I,const unsigned L);
		void add_part(const unsigned I,const unsigned D,const unsigned R);
		void add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W);
		PartedString& operator<<(const string& s);
		PartedString& operator<<(const unsigned i);
};
#endif
