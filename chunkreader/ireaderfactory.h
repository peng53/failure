#ifndef IREADER_FACTORY
#define IREADER_FACTORY
#include "ireader.h"
#include <vector>

using std::vector;

class IReaderFactory {
	private:
		vector<IReader*> inst;
	public:
		~IReaderFactory();
		IReader* ByFile(const string& filename,const size_t rate);
		IReader* ByInput(const string& input);
};

#endif
