#include <ncurses.h>
#include <sqlite3.h>
#include "ui.cpp"
#include <cstdio>

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
	Record t;
	build_a_record(0,COLS-31,t);
	erase();
	ins_table(db,t);
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db,"SELECT * FROM records",-1,&stmt,0);
	show_results(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	refresh();
	getch();
	endwin();
	cout << t << '\n';
	cout << t.uid << t.code <<'\n';
	return 0;
}