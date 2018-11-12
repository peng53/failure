#ifndef JSOn
#define JSOn

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
