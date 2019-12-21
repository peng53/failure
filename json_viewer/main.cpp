#include "app.h"
#include <iostream>

using std::cout;

void tostdout(const char* outstring){
	cout << outstring << '\n';
}


int main(int argc, char** argv){
	App t;
	View v;
	Jso* j = JSON::Num(10);
	v.setViewItem(j);
	v.setItemsPerPage(10);
	v.getItem(tostdout);
	return 0;
}
