#ifndef JSOn
#define JSOn
#include "jso.h"
#include "../chunkreader/chread.h"
#include <stack>
using std::stack;
class JSON {
	private:
		Jso* o;
		JSON(const JSON& rhs);
	public:
		JSON();
		~JSON();
		Jso* operator*();
		friend ostream& operator<<(ostream& out,const JSON& rhs);
};
char next_symplex(ChunkReader& chr);
void object_handler(stack<Jso*>& stk, ChunkReader& chr);
void array_handler(stack<Jso*>& stk, ChunkReader& chr);
#endif
