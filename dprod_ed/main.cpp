#include <ncurses.h>
#include <sqlite3.h>
#include <iostream>
#include "db.h"
#include "ui.h"

using std::cout;

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
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
	//Record a_record = editor.record_edit(0,COLS-31,r);
	int rnum = prompt_rnum();
	if (rnum!=-1){
		nRecord editor;
		Record a_record(db,rnum);
		r = editor.record_edit(0,COLS-31,a_record);
		if (r==0){
			mvprintw(3,3,a_record);
			getch();
		}
	}
	sqlite3_close(db);
	endwin();

	//~ if (r==0){
		//~ ins_table(db,t);
	//~ }
	//~ sqlite3_stmt* stmt;
	//~ sqlite3_prepare_v2(db,"SELECT * FROM records",-1,&stmt,0);
	//~ show_results(stmt);
	//~ sqlite3_finalize(stmt);
	//erase();
	//refresh();
	//endwin();
	//}

	return 0;
}
