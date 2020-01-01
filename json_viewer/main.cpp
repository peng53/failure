#include "app.h"
#include <iostream>

#include <unordered_map>

using std::to_string;
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

	const size_t numbersCnt = 26+1;
	Jso *numbers[numbersCnt];
	for (size_t i=0; i<numbersCnt; ++i){
		numbers[i] = JSON::Num(i);
		jm->Append(to_string(i), numbers[i]);
	}

	v.setViewItem(JSON::Single(JType::Null));
	v.setItemsPerPage(10);
	v.setViewItem(jm);
	cout << "---Page 1--\n";
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	v.nextPage();
	cout << "---Page 2--\n";
	while (v.state == PageState::MORE){
		v.getItem([] (string& s){ cout << s << '\n';});
	}
	delete jf;
	delete js;
	delete ja;
	delete jm;
	for (size_t i=0; i<numbersCnt; ++i){
		delete numbers[i];
	}

	return 0;
}
