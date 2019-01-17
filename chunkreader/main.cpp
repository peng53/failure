#include "ireader.h"
#include "ireader_extf.h"
#include "chread.h"
#include "stream_test.h"
#include <iostream>

using std::cout;

int main(){
	IReader *reader;
	
	cout << "BEGIN AReader test {\n";
	string poop = "    {   		  \"title\"		   	:   \"my property\"    }";
	reader = new AReader();
	reader->load(poop);
	reader->until('{');
	cout << closure(reader) << '\n';
	delete reader;
	cout << "} AReader test END\n";

	cout << "BEGIN ChunkReader test {\n";
	reader = new ChunkReader(80);
	reader->load("../in_out/test_json.json");
	reader2stdout(reader,cout);
	reader->load("../in_out/ncolon.json");
	reader2stdout(reader,cout);
	delete reader;
	cout << "} ChunkReader test END\n";

	return 0;
}
