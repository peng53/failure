#ifndef STREAM_TEST
#define STREAM_TEST
#include <iostream>
#include <fstream>
#include <string>

using std::streambuf;
using std::string;
using std::ostream;

class AReader {
	private:
		streambuf* t;
		char *ch;
		size_t I, E;
		const size_t M;
		bool good;
	public:
		AReader(streambuf* sb,const size_t csize); // constructs an AReader with a streambuf with data.
		AReader& operator++(); // advances to next character.
		~AReader(); // destructs AReader.
		void feed(); // pulls data from streambuf.
		char get(); // gets current character.
		bool has_data(); // returns whether there is any new data, one should advance/feed if false.
		char until(char c,string* str_ptr=nullptr);
		// advances AReader until get is c. if provided string ptr, output to it.
		friend ostream& operator<<(ostream& out,AReader& rhs); // posts data from AReader, then feeds.
};
#endif
