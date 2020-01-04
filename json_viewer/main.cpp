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
	(*master)->Append("mini-arr", JSON::Arr());
	Jso* myarr = (*master)->key_value("mini-arr");
	(*master)->Append("num", JSON::Num(11));
	myarr->Append(JSON::Num(24));
	Jso* subitem = (*myarr->x.a)[0];
	cout << subitem << '\n';
	v.setViewItem(myarr);
	v.setItemsPerPage(4);
	allPagesFromViewToStdOut(v);

	View subview = v.openNthItem(0);
	allPagesFromViewToStdOut(subview);

	Jso* mynum = (*master)->key_value("num");
	v.setViewItem(mynum);
	allPagesFromViewToStdOut(v);
	return 0;
}
