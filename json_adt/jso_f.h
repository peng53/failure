#ifndef JSOn
#define JSOn

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>

using std::string;
using std::vector;
using std::map;
using std::stack;
using std::pair;
using std::ostream;

enum class JType { Obj, Arr, Str, Num};

struct Jso {
	JType t;
	union {
		string* s;
		float f;
		vector<Jso*>* a;
		map<string,Jso*>* m;
	} x;
	Jso(float v);
	Jso(const string& v);
	Jso(JType j);
	void key_value(const string& k,Jso* v);
	void key_value(const string& k,const string& v);
	void key_value(const string& k,const float v);
	void key_value(const string& k,JType vt);
	void add_value(const Jso v);
	/*
	void add_value(const string& v);
	void add_value(const float v);
	void add_value(JType vt);
	void add_value(Jso* v);
	*/
	void set_value(float v);
	void set_value(const string& v);
	void print_depth(ostream& O, stack<pair<string,Jso*>>& stk);
	friend std::ostream& operator<<(std::ostream& out,const Jso J);
};
class JSON {
	private:
		Jso* o;
	public:
		JSON();
		JSON(const JSON& rhs);
		~JSON();
		void key_value(const string& k,Jso* v);
		void key_value(const string& k,const string& v);
		void key_value(const string& k,const float v);
		void key_value(const string& k,JType vt);
		void all_out(ostream& O);
		friend ostream& operator<<(ostream& O,const JSON& J);
};

#endif
