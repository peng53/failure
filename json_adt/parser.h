#ifndef PARSER
#define PARSER
#include "jso_f.h"
//#include "../chunkreader/chread.h"
#include "../chunkreader/stream_test.h"

template <class T>
char next_symplex(T& chr); // advances ChunkReader to next valid symbol and returns it.
template <class T>
JSON& parse_file(T& chr, JSON& lv); // parses ChunkReader, creating the structure in JSON lv.
#endif
