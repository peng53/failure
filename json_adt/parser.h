#ifndef PARSER
#define PARSER
#include "jso_f.h"
#include "../chunkreader/ireader.h"

char nextNonWS(IReader* buf);
// parses IReader, creating the structure in JSON lv.
JSON& parse_file_comma(IReader* buf, JSON& tree);
#endif
