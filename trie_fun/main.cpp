/** main.cpp tests the wordbank implentation.
 * Below shows construction, word adding, and use
 * of the 'prefix_exists' function.
 *
 * Build with `g++ main.cpp wordbank.cpp -std=c++11`
 */

#include "wordbank.h"
#include <iostream>

int main(){
	WordBank ws;
	std::string t  = "bar";
	ws.add_word(std::string("bar"));
	t  = "bat";
	ws.add_word(t);
	ws.add_word("cat");
	if (ws.prefix_exists(t)){
		std::cout<<"bar exists\n";
	} else {
		std::cout<<"bar dn exists\n";
	}
	t = "ball";
	if (ws.prefix_exists(t)){
		std::cout<<"ball exists\n";
	} else {
		std::cout<<"ball dn exists\n";
	}
	return 0;
}
