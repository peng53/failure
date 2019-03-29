#ifndef JSOn
#define JSOn
#include "jso.h"
class JSON {
	private:
		Jso* o; /*!< A Map Jso that contains child objects */
	public:
		JSON(); //!< JSON constructor
		~JSON(); //!< JSON destructor
		Jso* operator*(); //!< Allows dereferencing JSON for its stored Jso.
		/*! \name Factory method constructors for various Jso types
		 * Allows construction of a Jso from JSON. A Jso will need valid
		 * arguments to build the corresponding JType. E.g. a Str will need a
		 * string and Num a double. Map and Arr do not require any argument.
		 * null,true, and false are recieved from Single and will return a ptr
		 * to the Singleton of it.
		 */
		 //@{
		static Jso *Map(), *Arr();
		static Jso* Num(const double v);
		static Jso* Str(const string& v);
		static Jso* Single(JType t);
		//@{
		/** Jso Singleton */
		static Jso Null, True, False;
		//@}
};
ostream& operator<<(ostream& out,JSON& rhs);

#endif
