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
		friend ostream& operator<<(ostream& out,PartedString &PS);
		friend PartedString& operator<<(PartedString& p,const string& s);
		friend PartedString& operator<<(PartedString& p,const unsigned i);
	public:
		PartedString();
		~PartedString();
		size_t lits_size();
		void add_part(const string& s);
		void add_part(const unsigned I);
		void add_part(const unsigned I,const unsigned L);
		void add_part(const unsigned I,const unsigned D,const unsigned R);
		void add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W);
};
#endif
