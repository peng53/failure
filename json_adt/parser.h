#ifndef PARSER
#define PARSER
#include "jso_f.h"
#include "../chunkreader/chread.h"

char next_symplex(ChunkReader& chr); // advances ChunkReader to next valid symbol and returns it.
JSON& parse_file(ChunkReader& chr, JSON& lv); // parses ChunkReader, creating the structure in JSON lv.
#endif
