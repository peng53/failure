#ifndef CHKr
#define CHKr

#include <fstream>
#include <string>
#include <map>
#include <ostream>
#include <ios>

using std::string;
using std::ifstream;
using std::map;
using std::ostream;

class ChunkReader : ifstream {
	size_t I,E;
	const size_t M;
	char* ch;
	bool good;
	public:
	using ifstream::is_open;
	ChunkReader(const char* filename,const size_t csize);
	~ChunkReader();
	void feed();
	char until(char c,string* str_ptr=nullptr);
	string capture_until(char c);
	friend ostream& operator<<(ostream& out,ChunkReader& rhs);
	string& closure(string& s);
	string closure();
	void advance();
	char get();
	bool empty();
	bool has_data();
};

#endif
