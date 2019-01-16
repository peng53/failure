#ifndef PARSER
#define PARSER
#include "jso_f.h"
#include "../chunkreader/ireader.h"

char next_symplex(IReader* chr); // advances IReader to next valid symbol and returns it.
JSON& parse_file(IReader* chr, JSON& lv); // parses IReader, creating the structure in JSON lv.
#endif
