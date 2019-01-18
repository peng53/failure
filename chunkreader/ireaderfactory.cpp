#include "ireaderfactory.h"
#include "chread.h"
#include "stream_test.h"

IReaderFactory::~IReaderFactory(){
	for (const auto& ptr : inst){
		delete ptr;
	}
}
IReader* IReaderFactory::ByFile(const string& filename,const size_t rate){
	inst.emplace_back(new ChunkReader(rate));
	inst.back()->load(filename);
	return inst.back();
}
IReader* IReaderFactory::ByInput(const string& input){
	inst.emplace_back(new AReader);
	inst.back()->load(input);
	return inst.back();
}
