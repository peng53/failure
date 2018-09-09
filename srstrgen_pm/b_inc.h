#ifndef B_INC
#define B_INC
#include <string>
#include <cstring>
#include <algorithm>

using std::string;
using std::sort;

class BInc {
	private:
		string s;
	public:
		BInc(){}
		BInc(const string& I);
		const string& rep(){
			return s;
		}
		void rem(char* E);
		void add(const string& A);
};
#endif
