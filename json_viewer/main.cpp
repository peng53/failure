#include <iostream>
#include <ncurses.h>
#include "app.h"
#include "../json_adt/parser.h"
#include "../chunkreader/ireaderfactory.h"


using std::to_string;
using std::cout;

static void pageFromViewToStdout(View& v){
	while (v.state == PageState::MORE){
		v.getItem([] (const string& s){ cout << s << '\n';},[] (const string& s){ cout << s << '\n';});
	}
}

static void allPagesFromViewToStdOut(View &v){
	unsigned pageNum = 0;
	do {
		cout << "---Page " << pageNum++ << " ---\n";
		pageFromViewToStdout(v);
	} while (v.nextPage());
}

static void reader2jsonADT(IReader* textChunk, JSON& j){
	if (textChunk->empty()){
		throw std::invalid_argument("No input or non-existent file.");
	}
	if (nextNonWS(textChunk)!='{'){
		throw std::out_of_range("Could not find opening curly brace.");
	}
	textChunk->advance();
	parse_file_comma(textChunk,j);
}

int main(int argc, char** argv){
	JSON master;

	IReaderFactory reader_maker;
	IReader *textChunk;
	if (argc<2){
		string rawJson = R"~(
			{
				"mini-arr": [
					314e-2,
					2e4,
					-64,
					844,
					[
						"fox"
					]
				],
				"mini-map": {
					"a": "alpha",
					"b": "banana",
					"c": "cisco",
					"r" : {
						"rabbit" : true
					},
					"t" : 5
				}
			}
		)~";
			textChunk = reader_maker.ByInput(rawJson);
	} else {
		cout << "Using file: " << argv[1];
		textChunk = reader_maker.ByFile(argv[1],1024);
	}
	
	reader2jsonADT(textChunk, master);

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	View v;
	v.setViewItem(*master);
	v.setItemsPerPage(LINES);
	App a;
	a.setRootViewItem(v);
	a.setDimensions(COLS,LINES);
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
