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
	init_pair(1,COLOR_RED,COLOR_BLACK); //
	init_pair(2,COLOR_MAGENTA,COLOR_BLACK); // MENU disable
	init_pair(3,COLOR_CYAN,COLOR_BLACK); // results row
	init_pair(4,COLOR_YELLOW,COLOR_BLACK); // editor
	init_pair(5,COLOR_GREEN,COLOR_BLACK); //
	init_pair(6,COLOR_BLUE,COLOR_BLACK); //
	mainMenu mm;
	char s[100];
	int l=-1;
	sqlite3* db;
	while (l!=2){
		l = mm.run();
		switch (l){
			case 0:
				if (getAfileName(s)==0 && open_exdb(&db,s)==0){
					SQLi mydb(db);
					database_mnip(0,0,LINES-1,mydb);
					erase();
				}
				break;
			case 1:
				if (getAfileName(s)==0 && open_nwdb(&db,s)==0){
					SQLi mydb(db);
					mydb.def_table();
					database_mnip(0,0,LINES-1,mydb);
					erase();
				}
				break;
		}
	}
	endwin();
	return 0;
}
