// Count word prog

#include <iostream>
#include "wordbank.h"
#include <string>

using std::cout;

bool is_letter(char c){
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int main(int argc, char** argv){
	if (argc<2){
		return 1;
	}
	WordBank ws, ws2, ws3;
	ws += "cat";
	ws += "bat";
	ws += "mat";
	ws2 += "frogmanzoo";
	ws2 += "telegraph";
	ws2 += "alphastrike";
	ws2 += "catwarriorz";
	ws3 += "zebrararitytango";
	std::cout << "\nWordBank::ws\n" << ws << "\nWordBank::ws2\n" << ws2 << "\nWordBank::ws3\n" << ws3;
	ws << ws2 << ws3;
	std::cout << "\nWordBank::ws\n" << ws << "\nWordBank::ws2\n" << ws2 << "\nWordBank::ws3\n" << ws3;

	return 0;
}
