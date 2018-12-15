#ifndef CHKr
#define CHKr

#include <fstream>
#include <string>
#include <ostream>

using std::string;
using std::ifstream;
using std::ostream;

class ChunkReader : ifstream {
	private:
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
		void advance();
		char get();
		bool empty();
		bool has_data();
		string& closure(string& s);
		string closure();
};

#endif
