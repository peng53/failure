#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>
#include "db.h"
#include "ui.h"

using std::cout;

int main(){
	initscr();
	noecho();
	start_color();
	int r(-1);
	int l(1);
	init_pair(1,COLOR_RED,COLOR_BLACK); //
	init_pair(2,COLOR_MAGENTA,COLOR_BLACK); // MENU disable
	init_pair(3,COLOR_CYAN,COLOR_BLACK); // results row
	init_pair(4,COLOR_YELLOW,COLOR_BLACK); // editor
	mainMenu mm;
	char s[100];
	sqlite3* db;
	while (l){
		r = mm.run();
		switch (r){
			case 0:
				if (getAfileName(s)==0 && open_exdb(&db,s)==0){
					SQLi mydb(db);
					move(0,0);
					printw("del|ins|view|edit|close|save|refr");
					attron(A_UNDERLINE);
					mvaddch(0,0,'d');
					mvaddch(0,4,'i');
					mvaddch(0,8,'v');
					mvaddch(0,13,'e');
					mvaddch(0,18,'c');
					mvaddch(0,24,'s');
					mvaddch(0,29,'r');
					attroff(A_UNDERLINE);
					database_mnip(1,0,LINES-2,mydb);
					erase();
				}
				break;
			case 1:
				if (getAfileName(s)==0 && open_nwdb(&db,s)==0){
					SQLi mydb(db);
					mydb.def_table();\
					database_mnip(1,0,LINES-2,mydb);
					erase();
				}
				break;
			case 2: l = 0; break;

		}
	}
	endwin();
	return 0;
}
