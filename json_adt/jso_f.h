#ifndef JSOn
#define JSOn
#include "jso.h"
class JSON {
	private:
		Jso* o;
		JSON(const JSON& rhs);
	public:
		JSON();
		~JSON();
		Jso* operator*();
};

#endif
