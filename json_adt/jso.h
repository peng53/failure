#ifndef JSOnObj
#define JSOnObj

#include <string>
#include <vector>
#include <map>
#include <iostream>

using std::string;
using std::vector;
using std::map;
using std::ostream;

enum class JType { Obj, Arr, Str, Num};

struct Jso {
	JType t;
	union {
		string* s;
		double f;
		vector<Jso*>* a;
		map<string,Jso*>* m;
	} x;
	explicit Jso(double v);
	explicit Jso(const string& v);
	explicit Jso(JType j);
	~Jso();
	void key_value(const string& k,Jso* v);
	void key_value(const string& k,const string& v);
	void key_value(const string& k,const double v);
	void key_value(const string& k,JType vt);
	void add_value(const double v);
	void add_value(const string& v);
	void add_value(const JType vt);
	void add_value(Jso* v);
	void set_value(double v);
	void set_value(const string& v);
	friend ostream& operator<<(ostream& out,const Jso& J);
	Jso* key_value(const string& k);
	void rprint(ostream& out, const string& label);
};
void indent_it(size_t i,ostream& out);
#endif
