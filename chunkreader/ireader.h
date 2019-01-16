#ifndef IREADER
#define IREADER

#include <string>

using std::string;

class IReader {
	protected:
		bool good;
		const size_t M;
		explicit IReader(const int chars): good(true), M(chars){}
	public:
		virtual ~IReader(){}
		virtual IReader& operator++() = 0;
		virtual void advance() = 0;
		virtual void feed() = 0;
		virtual char get() = 0;
		virtual bool has_data() = 0;
		virtual char until(char c,string* str_ptr=nullptr) = 0;
		virtual bool empty() = 0;
};

#endif
