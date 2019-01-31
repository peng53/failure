#ifndef JSOnObj
#define JSOnObj

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using std::string;
using std::vector;
using std::unordered_map;
using std::ostream;
using std::pair;

enum class JType { Obj, Arr, Str, Num, Null, True, False};

struct Jso {
	JType t;
	union {
		string* s;
		double f;
		vector<Jso*>* a;
		unordered_map<string,Jso*>* m;
	} x;
	explicit Jso(JType);
	~Jso();

	void Append(const string& k,Jso* v);
	void Append(const pair<string,Jso*>& kv);
	void Append(Jso* v);

	Jso* key_value(const string& k);
	//void rprint(ostream& out, const string& label);
	//explicit operator const string&();
	Jso* operator[](const string& k);

	void Get(string **outptr);
	void Get(double **outptr);
	void Get(unordered_map<string,Jso*> **outptr);
	void Get(vector<Jso*> **outptr);
	void SetVoid(double v=0);
	string* Str();
};
ostream& operator<<(ostream& out,const Jso& J);
void tprint(ostream& out, Jso* root);
#endif
