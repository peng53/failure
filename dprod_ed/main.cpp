#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>
#include "db.h"
#include "ui.h"

using std::cout;

int main(){
	initscr();
	start_color();
	int r(-1);
	int l(1);
	init_pair(1,COLOR_RED,COLOR_BLACK); //
	init_pair(2,COLOR_MAGENTA,COLOR_BLACK); // MENU disable
	mainMenu mm;
	char s[100];
	sqlite3* db;
	while (l){
		r = mm.run();
		switch (r){
			case 0:
				if (getAfileName(s)==0 && open_exdb(&db,s)==0){
					SQLi mydb(db);
					database_mnip(0,0,LINES-2,mydb);
				}
				break;
			case 1:
				if (getAfileName(s)==0 && open_nwdb(&db,s)==0){
					SQLi mydb(db);
					mydb.def_table();
					database_mnip(0,0,LINES-2,mydb);
				}
				break;
			case 2: l = 0; break;

		}
	}
	endwin();
	return 0;
}
