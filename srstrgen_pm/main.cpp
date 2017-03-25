#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "st.cpp"

using std::cout;
using std::vector;
using std::list;
using std::string;

int main(){
	list<Part*> ps;
	vector<string*> lits;
	lits.push_back(new string {"601"});
	lits.push_back(new string {"hello"});
	lits.push_back(new string {"-"});
	ps.push_back(new CPart(lits[1]));
	ps.push_back(new CPart(lits[2]));
	ps.push_back(new RPart(lits[0],4,lits[2],3));
	for (auto p: ps){
		cout << *p;
	}
	cout << '\n';
	for (auto l : lits){
		delete l;
	}
	for (auto p : ps){
		delete p;
	}
	return 0;
}
