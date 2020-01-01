#include "app.h"
#include <iostream>

#include <unordered_map>

using std::to_string;
using std::cout;
using std::unordered_map;


int main(int argc, char** argv){
	View v;
	JSON master;
	
	const size_t numbersCnt = 26+1;
	for (size_t i=0; i<numbersCnt; ++i){
		(*master)->Append(to_string(i), JSON::Num(i));
	}
	(*master)->Append("mini-map", JSON::Map());
	(*master)->Append("mini-arr", JSON::Arr());

	v.setViewItem(*master);
	v.setItemsPerPage(4);
	
	unsigned pageNum = 0;
	do {
		cout << "---Page " << pageNum++ << " ---\n";
		while (v.state == PageState::MORE){
			v.getItem([] (string& s){ cout << s << '\n';});
		}
	} while (v.nextPage());
	
	return 0;
}
