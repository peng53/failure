#ifndef PARSER
#define PARSER
#include "jso_f.h"
#include "../chunkreader/ireader.h"

char nextNonWS(IReader* buf);
// Returns the next character from IReader that isn't considered whitespace.
JSON& parse_file_comma(IReader* buf, JSON& tree);
// Parses IReader for JSON data.
#endif
