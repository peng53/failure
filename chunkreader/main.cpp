#include "ireader.h"
#include "chread.h"
#include "stream_test.h"
#include <iostream>

using std::cout;

void reader2stdout(IReader* reader){
	while (!reader->empty()){
		cout << reader->get();
		reader->advance();
	}
	cout << '\n';
}

int main(){
	cout << "BEGIN AReader test {\n";
	string poop = "    {   		  \"title\"		   	:   \"my property\"    }";
	IReader *reader = new AReader(poop);
	reader->until('{');
	reader2stdout(reader);
	delete reader;
	cout << "} AReader test END\n";

	cout << "BEGIN ChunkReader test {\n";
	//reader = new ChunkReader("../in_out/test_json.json",80);
	reader = new ChunkReader("../in_out/test_json.json",80);
	reader2stdout(reader);
	delete reader;
	cout << "} ChunkReader test END\n";
	return 0;
}
