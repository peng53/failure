#ifndef CHKr
#define CHKr

#include <fstream>
#include <string>
#include <map>
#include <ostream>

using std::string;
using std::ifstream;
using std::map;
using std::ostream;

class ChunkReader : ifstream {
	size_t i, b;
	const size_t S;
	char* ch;
	public:
	ChunkReader(const char* filename,const size_t csize);
	~ChunkReader();
	void feed();
	string get_capture();
	string& get_capture(string& s);
	string& capture_until(string& s, char c);
	string capture_until(char c);
	string& closure(string& s);
	char at();
	void advance();
	bool empty();
	char until(char c);
	friend ostream& operator<<(ostream& out,ChunkReader& chk);
};

#endif
