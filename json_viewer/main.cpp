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
	JSON master;
	cout << "Master JSON was initilized!\n";
	(*master)->Append("mini-arr", JSON::Arr());
	cout << "mini-array appended to master.\n";
	(*master)->Append("mini-map", JSON::Map());
	cout << "mini-map appended to master.\n";

	Jso* myarr = (*master)->key_value("mini-arr");
	myarr->Append(JSON::Num(24));
	cout << "24 was appended to mini-array as a number\n\n";
	for (unsigned i = 25; i < 70; ++i){
		myarr->Append(JSON::Num(i));
	}

	Jso* mymap = (*master)->key_value("mini-map");
	mymap->Append("300", JSON::Str("300!!!!!"));
	cout << "300, 300!!!!! was appended to mini-map as a str,str\n\n";
	for (unsigned i = 25; i < 70; ++i){
		mymap->Append(std::to_string(i), JSON::Num(i));
	}
	/*
	cout << "--master items!--\n";
	allPagesFromViewToStdOut(v);
	cout << "------------\n";
	cout << "\nmaster items was viewed, it contains mini-arr and mini-map\n";

	View subview = v.openNthItem(1);
	cout << "subview created for 1st item of master\n\n";
	cout << "--subview items!--\n";
	allPagesFromViewToStdOut(subview);
	cout << "------------\n";
	cout << "\nsubview was viewed, it only contains 24 or 300\n";
	
	subview = subview.openNthItem(0);
	allPagesFromViewToStdOut(subview);
	v.reloadPage();
	*/
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	View v;
	v.setViewItem(*master);
	v.setItemsPerPage(LINES);
	App a;
	a.setRootViewItem(v);
	a.setDimensions(24,LINES);
	a.addBind('q', Command::QUIT);
	a.addBind('r', Command::RELOAD);
	a.addBind(KEY_DOWN, Command::MOVEDOWN);
	a.addBind(KEY_UP, Command::MOVEUP);
	a.addBind('c', Command::CLOSEVIEW);
	a.addBind('\n', Command::OPENVIEW);
	a.addBind(KEY_NPAGE, Command::NEXTPG);
	a.addBind(KEY_PPAGE, Command::PREVPG);
	a.draw();
	while (a.running){
		a.keySym(getch());
	}
	endwin();
	return 0;
}
