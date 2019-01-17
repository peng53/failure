#ifndef IREADER_EXTF
#define IREADER_EXTF
#include "ireader.h"
#include <iostream>
#include <unordered_map>

using std::unordered_map;
using std::ostream;

void reader2stdout(IReader* reader,ostream& out);
template <typename T>
T& closure(IReader* reader,T& output,unordered_map<char,char>* match=nullptr);
string closure(IReader* reader);
#endif
