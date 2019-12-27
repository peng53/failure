#include "app.h"
#include <iostream>

#include <unordered_map>

using std::cout;
using std::unordered_map;


int main(int argc, char** argv){
	View v;
	Jso* jf = JSON::Num(10);
	Jso* js = JSON::Str("hello world");
	Jso* ja = JSON::Arr();
	Jso* jm = JSON::Map();

	ja->Append(jf);
	ja->Append(js);
	jm->Append("arr", ja);
	jm->Append("str", js);
	jm->Append("num", jf);

	v.setViewItem(JSON::Single(JType::Null));
	v.setItemsPerPage(10);
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	v.setViewItem(jm);
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	delete jf;
	delete js;
	delete ja;
	delete jm;

	return 0;
}
