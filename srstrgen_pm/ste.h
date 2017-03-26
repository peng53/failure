#include <ostream>
#include <string>
#include <random>
#include <vector>
#include <list>
#include <memory>

using std::ostream;
using std::string;
using std::list;
using std::unique_ptr;

class PartedString {
	private:
		struct Part;
		struct CPart;
		struct RPart;
		struct DPart;
		list<unique_ptr<Part>> parts;
		std::vector<unique_ptr<string>> lits;
		void add_lit(const string& s);
		friend ostream& operator<<(ostream& out,PartedString &PS);
		friend PartedString& operator<<(PartedString& p,const string& s);
		friend PartedString& operator<<(PartedString& p,const unsigned i);
	public:
		unsigned lits_size(){
			return lits.size();
		}
		void add_part(const string& s);
		void add_part(const unsigned I);
		void add_part(const unsigned I,const unsigned L);
		void add_part(const unsigned I,const unsigned D,const unsigned R);
		void add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W);
};
