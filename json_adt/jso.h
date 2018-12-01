#ifndef JSOnObj
#define JSOnObj

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>

using std::tie;
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
	explicit Jso(float v);
	explicit Jso(const string& v);
	explicit Jso(JType j);
	~Jso();
	void key_value(const string& k,Jso* v);
	void key_value(const string& k,const string& v);
	void key_value(const string& k,const float v);
	void key_value(const string& k,JType vt);
	void add_value(const float v);
	void add_value(const string& v);
	void add_value(const JType vt);
	void add_value(Jso* v);
	void set_value(float v);
	void set_value(const string& v);
	friend std::ostream& operator<<(std::ostream& out,const Jso& J);
	Jso* key_value(const string& k);
	void jso_out(ostream& out);
};
void all_out(Jso j,ostream& out);
void indent_it(size_t i,ostream& out);
#endif
