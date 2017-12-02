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
	sqlite3* db;
	nRecord editor;
	Record rec;
	bool db_open(0);
	char s[100];
	sqlite3_stmt* stmt;
	while (l){
		r = mm.run();
		switch (r){
			case 0:
				if (db_open){
					// error
					break;
				}
				r = getAfileName(s);
				if (r==0){ // if file is good
					r = open_exdb(&db,s);
					if (r==0){
						db_open = 1;
						mm.more_ops(1);
						sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
					}
				}
				break;
			case 1:
				if (db_open){
					sqlite3_close(db);
				}
				r = getAfileName(s);
				if (r==0){ // if file is good
					r = open_nwdb(&db,s);
					if (r==0){
						def_table(db);
						db_open = 1;
						mm.more_ops(1);
						sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
						sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
					}
				}
				break;
			case 2:
				if (db_open){ break; }
				l = 0; break;
			case 4:
				editor.depopulate();
				r = editor.edit(0,COLS-31);
				if (r==0){ //good record
					editor.exportr(rec);
					ins_table(db,rec);
					mvprintw(3,3,rec);
					getch();
					erase();
				}
				break;
			case 5:
				sqlite3_prepare_v2(db,"SELECT * FROM records",-1,&stmt,0);
				show_results(stmt);
				getch();
				erase();
				break;
			case 6:
				r = prompt_rnum();
				rec = Record(db,r);
				if (rec.rnum==r){
					editor.depopulate();
					editor.populate(rec);
					r = editor.edit(0,COLS-31);
					if (r==0){
						editor.exportr(rec);
						//update_table(db,rec);
						mvprintw(3,3,rec);
						getch();
						erase();
					}
				}
				break;
			case 7:
				// delete_table(db,rec);
				break;
			case 8:
				sqlite3_close(db);
				db_open = 0;
				mm.more_ops(0);
				break;
			case 9:
				sqlite3_exec(db,"END TRANSACTION;",NULL,NULL,NULL);
				sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
				break;
		}
	}
	endwin();
	sqlite3_finalize(stmt);
	if (db_open){ sqlite3_close(db); }

	return 0;
}
