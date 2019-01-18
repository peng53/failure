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

enum class JType { Obj, Arr, Str, Num, Null, True, False};

struct Jso {
	JType t;
	union {
		string* s;
		double f;
		vector<Jso*>* a;
		map<string,Jso*>* m;
	} x;
	Jso(){}
	~Jso();

	void Append(const string& k,Jso* v);
	void Append(Jso* v);

	friend ostream& operator<<(ostream& out,const Jso& J);
	Jso* key_value(const string& k);
	void rprint(ostream& out, const string& label);
	explicit operator const string&();
	Jso* operator[](const string& k);

	void Get(string **outptr);
	void Get(double **outptr);
	void Get(map<string,Jso*> **outptr);
	void Get(vector<Jso*> **outptr);
};

#endif
