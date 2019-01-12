#ifndef STREAM_TEST
#define STREAM_TEST
#include <iostream>
#include <fstream>
#include <string>
#include "ireader.h"

using std::streambuf;
using std::string;
using std::ostream;

class AReader : public IReader {
	private:
		streambuf* t;
		char *ch;
		size_t I, E;

	public:
		AReader(streambuf* sb,const size_t csize); // constructs an AReader with a streambuf with data.
		~AReader(); // destructs AReader.
		AReader& operator++() override; // advances to next character.
		void advance() override;
		void feed() override; // pulls data from streambuf.
		char get() override; // gets current character.
		bool has_data() override; // returns whether there is any new data, one should advance/feed if false.
		bool empty() override;
		char until(char c,string* str_ptr=nullptr) override;
		// advances AReader until get is c. if provided string ptr, output to it.
		friend ostream& operator<<(ostream& out,AReader& rhs); // posts data from AReader, then feeds.
};
#endif
