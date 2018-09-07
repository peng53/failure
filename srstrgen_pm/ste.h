#ifndef MY_ST
#define MY_ST
#include <ostream>
#include <string>

using std::ostream;
using std::string;

class PartedString {
	private:
		struct IMPL;
		IMPL* M;
		friend ostream& operator<<(ostream& out,const PartedString &PS);
	public:
		PartedString();
		~PartedString();
		size_t lits_size() const;
		PartedString& operator<<(const string& s);
		void add_part(const string& s);
		PartedString& operator<<(const size_t i);
		void add_part(const size_t index);
		void add_part(const size_t index,const size_t count);
		void add_part(const size_t index,const size_t delimORend,const size_t repeat);
		void add_part(const size_t index,const size_t count,const size_t delim,const size_t repeat);
};
#endif
