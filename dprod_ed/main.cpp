#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>
#include "db.h"
#include "ui.h"

using std::cout;

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	sqlite3 *db;
	char s[100];
	int r = getAfileName(s);
	if (r==2){
		endwin();
		return 1;
	}
	r = open_exdb(&db,s);
	if (r==1){
		open_nwdb(&db,s);
		def_table(db);
	}
	int rnum = prompt_rnum();
	if (rnum!=-1){
		Record t(db,rnum);
		if (t.rnum!=-1){
			cbreak();
			noecho();
			r = make_rec_win(0,COLS-31,t);
		}
	}
	erase();
	//~ if (r==0){
		//~ ins_table(db,t);
	//~ }
	//~ sqlite3_stmt* stmt;
	//~ sqlite3_prepare_v2(db,"SELECT * FROM records",-1,&stmt,0);
	//~ show_results(stmt);
	//~ sqlite3_finalize(stmt);
	sqlite3_close(db);
	refresh();
	endwin();
	return 0;
}
