#include <iostream>
#include <string>
#include "st.cpp"

using std::cout;
using std::string;

int main(){
	PartedString M;
	string s0 {"601"};
	string s1 {"hello"};
	string s2 {"-"};
	string s3 {"gmail"};
	string s4 {"hotmail"};
	string s5 {"yahoo"};
	M.add_lit(s0);
	M.add_lit(s1);
	M.add_lit(s2);
	M.add_lit(s3);
	M.add_lit(s4);
	M.add_lit(s5);
	M.add_part(1);
	M.add_part(2);
	M.add_part(0,4,2,3);
	M.add_part(string {"@"});
	M.add_part(3,5,0);
	cout << M << '\n';
	return 0;
}
