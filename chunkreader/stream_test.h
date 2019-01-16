#ifndef STREAM_TEST
#define STREAM_TEST
#include "ireader.h"
#include <ostream>

using std::ostream;

class AReader : public IReader {
	private:
		string chars;
		size_t i;
	public:
		AReader(const string& input); // constructs an AReader with a streambuf with data.
		AReader& operator++() override; // advances to next character.
		void advance() override;
		void feed() override;
		char get() override; // gets current character.
		bool has_data() override; // returns whether there is any new data, one should advance/feed if false.
		bool empty() override;
		char until(char c,string* str_ptr=nullptr) override;
		// advances AReader until get is c. if provided string ptr, output to it.
};
#endif
