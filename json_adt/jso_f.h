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
		static Jso *Null(), *True(), *False(), *Map(), *Arr();
		static Jso* Num(const double v);
		static Jso* Str(const string& v);
		static Jso* Single(JType t);
};

#endif
