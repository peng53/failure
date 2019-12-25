#include "app.h"
#include <iostream>

using std::cout;


int main(int argc, char** argv){
	View v;
	Jso* jf = JSON::Num(10);
	Jso* js = JSON::Str("hello world");
	Jso* ja = JSON::Arr();
	ja->Append(jf);
	ja->Append(js);
	v.setViewItem(jf);
	v.setItemsPerPage(10);
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	v.setViewItem(ja);
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	delete jf;
	delete js;
	delete ja;
	return 0;
}
