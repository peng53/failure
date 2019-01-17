#ifndef IREADER_EXTF
#define IREADER_EXTF
#include "ireader.h"
#include <iostream>
#include <map>

using std::map;
using std::ostream;

void reader2stdout(IReader* reader,ostream& out);
string& closure(IReader* reader,string& output);
string closure(IReader* reader);
#endif
