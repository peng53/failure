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
		static Jso *Null(), *True(), *False();
		Jso* Num(const double v);
		Jso* Str(const string& v);
		Jso* Map();
		Jso* Arr();
		Jso* Single(JType t);
};

#endif
