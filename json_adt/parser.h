#ifndef PARSER
#define PARSER
#include "jso_f.h"
//#include "../chunkreader/chread.h"
//#include "../chunkreader/stream_test.h"
#include "../chunkreader/ireader.h"

char next_symplex(IReader* chr); // advances ChunkReader to next valid symbol and returns it.
JSON& parse_file(IReader* chr, JSON& lv); // parses ChunkReader, creating the structure in JSON lv.
#endif
