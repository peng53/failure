#include <iostream>
#include "app.h"
#include "../json_adt/parser.h"
#include "../chunkreader/ireaderfactory.h"


using std::to_string;
using std::cout;

void pageFromViewToStdout(View& v){
	while (v.state == PageState::MORE){
		v.getItem([] (const string& s){ cout << s << '\n';});
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

	IReaderFactory reader_maker;
	string rawJson = R"~({ "mini-arr": [1, 2, 3, 4, 5, 6, 7, 8 ], "mini-map": {"a":"alpha", "b": "banana", "c": "cisco"}})~";
	IReader *textChunk = reader_maker.ByInput(rawJson);
	
	if (textChunk->empty()){
		throw std::invalid_argument("No input or non-existent file.");
	}
	if (nextNonWS(textChunk)!='{'){
		throw std::out_of_range("Could not find opening curly brace.");
	}
	textChunk->advance();
	parse_file_comma(textChunk,master);

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
	a.addBind(KEY_LEFT, Command::CLOSEVIEW);
	a.addBind('\n', Command::OPENVIEW);
	a.addBind(KEY_RIGHT, Command::OPENVIEW);
	a.addBind(KEY_NPAGE, Command::NEXTPG);
	a.addBind(KEY_PPAGE, Command::PREVPG);
	a.draw();
	while (a.running()){
		a.keySym(getch());
	}
	endwin();
	return 0;
}
