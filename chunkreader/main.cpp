#include "ireaderfactory.h"
#include "ireader_extf.h"
#include <iostream>

using std::cout;

int main(){
	IReaderFactory reader_builder;
	IReader *reader;
	
	cout << "BEGIN AReader test {\n";
	string poop = "    {   		  \"title\"		   	:   \"my property\"    }";
	reader = reader_builder.ByInput(poop);
	reader->until('{');
	cout << closure(reader) << '\n';
	cout << "} AReader test END\n";

	cout << "BEGIN ChunkReader test {\n";
	reader = reader_builder.ByFile("../in_out/test_json.json",80);
	reader2stdout(reader,cout);
	reader->load("../in_out/ncolon.json");
	reader2stdout(reader,cout);
	cout << "} ChunkReader test END\n";
	return 0;
}
