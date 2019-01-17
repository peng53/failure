#ifndef IREADER
#define IREADER
#include <string>

using std::string;

class IReader {
	protected:
		bool good;
		explicit IReader(): good(false){}
	public:
		virtual ~IReader(){}
		virtual IReader& operator++() = 0;
		virtual void advance() = 0;
		virtual bool empty() = 0;
		virtual void feed() = 0;
		virtual char get() = 0;
		virtual void load(const string& input) = 0;
		virtual char until(char c,string* str_ptr=nullptr) = 0;
};
#endif
