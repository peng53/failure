/** main.cpp tests the wordbank implentation.
 * Below shows construction, word adding, and use
 * of the 'prefix_exists' function.
 *
 * Build with `g++ main.cpp wordbank.cpp -std=c++11`
 */

#include "wordbank.h"
#include "../srstrgen_pm/st.h"
#include <iostream>
#include <sstream>

int main(){
	WordBank ws;
	PartedString ps;
	ps << "ABCD";
	ps << "1234567890";
	ps << "z";
	ps.add_part(0,3);
	ps.add_part(2);
	ps.add_part(1,3,2,2);
	//std::cout << ps;
	std::ostringstream out;
	out << ps;
	string s = out.str();
	ws << s;
	std::cout << ws;
	return 0;
}
