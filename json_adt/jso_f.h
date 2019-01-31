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
		static Jso *Map(), *Arr();
		static Jso* Num(const double v);
		static Jso* Str(const string& v);
		static Jso* Single(JType t);
		static Jso Null, True, False;
};
ostream& operator<<(ostream& out,JSON& rhs);

#endif
