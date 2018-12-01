#ifndef JSOn
#define JSOn
#include "jso.h"
#include "../chunkreader/chread.h"
class JSON {
	private:
		Jso* o;
		JSON(const JSON& rhs);
	public:
		JSON();
		~JSON();
		Jso* operator*();
};
char next_symplex(ChunkReader& chr);
void object_handler(stack<Jso*>& stk, ChunkReader& chr);
void array_handler(stack<Jso*>& stk, ChunkReader& chr);
#endif
