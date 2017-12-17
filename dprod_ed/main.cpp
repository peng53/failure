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
		if (l!=2){
			//< Where l==0 is existing, 1 is new.
			if (getAfileName(s)==0 && open_sqdb(&db,s,l)==0){
				SQLi mydb(db,l);
				database_mnip(0,0,LINES-1,mydb);
				erase();
			} else {
				printw((l==0) ? "Error: File doesn't exists?" : "Error: File already exists?");
			}
		}
	}
	endwin();
	return 0;
}
