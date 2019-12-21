#include "app.h"
#include <iostream>

using std::cout;


int main(int argc, char** argv){
	App t;
	View v;
	Jso* j = JSON::Num(10);
	Jso* js = JSON::Str("hello world");
	v.setViewItem(j);
	v.setItemsPerPage(10);
	v.getItem([] (string& s){ cout << s << '\n';});
	v.setViewItem(js);
	v.getItem([] (string& s){ cout << s << '\n';});
	delete j;
	delete js;
	return 0;
}
