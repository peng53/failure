#include "app.h"
#include <iostream>

#include <unordered_map>

using std::to_string;
using std::cout;
using std::unordered_map;

void pageFromViewToStdout(View& v){
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
}

void allPagesFromViewToStdOut(View &v){
	unsigned pageNum = 0;
	do {
		cout << "---Page " << pageNum++ << " ---\n";
		pageFromViewToStdout(v);
	} while (v.nextPage());
}

int main(int argc, char** argv){
	View v;
	JSON master;
	cout << "Master JSON was initilized!\n";
	(*master)->Append("mini-arr", JSON::Arr());
	cout << "mini-array appended to master.\n";

	Jso* myarr = (*master)->key_value("mini-arr");
	myarr->Append(JSON::Num(24));
	cout << "24 was appended to mini-array as a number\n\n";


	v.setViewItem(*master);
	v.setItemsPerPage(4);
	cout << "--master items!--\n";
	allPagesFromViewToStdOut(v);
	cout << "------------\n";
	cout << "\nmaster items was viewed, it only contains mini-arr\n";

	View subview = v.openNthItem(0);
	cout << "subview created for 0th item of master\n\n";
	cout << "--subview items!--\n";
	allPagesFromViewToStdOut(subview);
	cout << "------------\n";
	cout << "\nsubview was viewed, it only contains 24\n";
	return 0;
}
