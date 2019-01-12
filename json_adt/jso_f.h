#ifndef JSOn
#define JSOn
#include "jso.h"
class JSON {
	private:
		Jso* o;
	public:
		JSON();
		~JSON();
		Jso* operator*();
		friend ostream& operator<<(ostream& out,const JSON& rhs);
};
#endif
