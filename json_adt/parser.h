#ifndef PARSER
#define PARSER
#include "jso_f.h"
#include "../chunkreader/chread.h"

char next_symplex(ChunkReader& chr);
JSON& parse_file(ChunkReader& chr, JSON& lv);
#endif
