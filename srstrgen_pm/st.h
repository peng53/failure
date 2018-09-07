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
using std::unique_ptr;

struct Part {
	virtual void out(ostream& sout) const = 0;
};
class PartedString {
	private:
		vector<unique_ptr<string>> lits;
		std::list<unique_ptr<Part>> parts;
		friend ostream& operator<<(ostream& out,const PartedString &PS);
		void add_lit(const string& s);
	public:
		PartedString(){}
		~PartedString(){}
		size_t lits_size() const;
		void add_part(const string& s);
		void add_part(const size_t index);
		void add_part(const size_t index,const size_t count);
		void add_part(const size_t indexORstart,const size_t delimORend,const size_t repeat);
		void add_part(const size_t index,const size_t count,const size_t delim,const size_t repeat);
		PartedString& operator<<(const string& s);
		PartedString& operator<<(const size_t index);
};
#endif
