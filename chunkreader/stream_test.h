#ifndef STREAM_TEST
#define STREAM_TEST
#include "ireader.h"

class AReader : public IReader {
	private:
		string chars;
		string::iterator i;
	public:
		AReader& operator++() override; // advances to next character.
		void advance() override;
		bool empty() override;
		void feed() override;
		char get() override; // gets current character.
		void load(const string& input);
		char until(char c,string* str_ptr=nullptr) override;
		// advances AReader until get is c. if provided string ptr, output to it.
};
#endif
