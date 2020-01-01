#include "app.h"
#include <iostream>

#include <unordered_map>

using std::to_string;
using std::cout;
using std::unordered_map;

void allPagesFromViewToStdOut(View &v){
	unsigned pageNum = 0;
	do {
		cout << "---Page " << pageNum++ << " ---\n";
		while (v.state == PageState::MORE){
			v.getItem([] (string& s){ cout << s << '\n';});
		}
	} while (v.nextPage());
}

int main(int argc, char** argv){
	View v;
	JSON master;
	
	const size_t numbersCnt = 26+1;
	for (size_t i=0; i<numbersCnt; ++i){
		(*master)->Append(to_string(i), JSON::Num(i));
	}
	(*master)->Append("mini-map", JSON::Map());
	(*master)->Append("mini-arr", JSON::Arr());
	Jso* myarr = (*master)->key_value("mini-arr");
	myarr->Append(JSON::Single(JType::Null));
	myarr->Append(JSON::Single(JType::True));
	myarr->Append(JSON::Single(JType::False));
	myarr->Append(JSON::Num(24));
	myarr->Append(JSON::Str("twenty-four"));
	myarr->Append(JSON::Arr());

	v.setViewItem(*master);
	v.setItemsPerPage(4);

	cout << "Print out of master JSON\n";
	allPagesFromViewToStdOut(v);
	cout << "--------------\n";
	v.setViewItem(myarr);
	cout << "Print out of mini-arr\n";
	allPagesFromViewToStdOut(v);
	cout << "--------------\n";
	return 0;
}
