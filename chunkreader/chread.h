#ifndef CHKr
#define CHKr
#include "ireader.h"
#include <fstream>
#include <ostream>

using std::ifstream;
using std::ostream;

class ChunkReader : public IReader, ifstream {
	private:
		size_t I,E;
		char* ch;
	public:
		using ifstream::is_open;
		ChunkReader(const char* filename,const size_t csize);
		char until_e(const char c,const bool int_escape=false,string* str_ptr=nullptr);
		string capture_until(const char c);
		friend ostream& operator<<(ostream& out,ChunkReader& rhs);

		~ChunkReader();
		char until(const char c,string* str_ptr=nullptr) override;
		void feed() override;
		void advance() override;
		ChunkReader& operator++() override;
		char get() override;
		bool empty() override;
		bool has_data() override;

		string& closure(string& s);
		string closure();
};

#endif
