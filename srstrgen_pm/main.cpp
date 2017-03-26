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
	lits.push_back(new string {"gmail"});
	lits.push_back(new string {"hotmail"});
	lits.push_back(new string {"yahoo"});
	ps.push_back(new CPart(lits[1]));
	ps.push_back(new CPart(lits[2]));
	ps.push_back(new RPart(lits[0],4,lits[2],3));
	DPart* tempD = new DPart();
	tempD->f.push_back(lits[3]);
	tempD->f.push_back(lits[4]);
	tempD->f.push_back(lits[5]);
	ps.push_back(tempD);
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
