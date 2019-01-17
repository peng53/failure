#ifndef CHKr
#define CHKr
#include "ireader.h"
#include <fstream>
#include <ostream>

using std::ifstream;
using std::ostream;

class ChunkReader : public IReader, ifstream {
	private:
		const size_t M;
		size_t I,E;
		char* ch;
	public:
		using ifstream::is_open;
		ChunkReader(const size_t csize);
		ChunkReader(const string& filename,const size_t csize);
		~ChunkReader();
		ChunkReader& operator++() override;
		void advance() override;
		bool empty() override;
		void feed() override;
		char get() override;
		bool has_data();
		void load(const string& input) override;
		char until(const char c,string* str_ptr=nullptr) override;

		friend ostream& operator<<(ostream& out,ChunkReader& rhs);
};

#endif
