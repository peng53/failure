#ifndef D_UI
#define D_UI
#include <ncurses.h>
#include <form.h>
#include "db.h"

struct nRecord {
	FORM* F;
	FIELD* f[15];
	nRecord();
	~nRecord();
	void dress_rec_win(WINDOW* W,int rnum);
	time_t get_start_time();
	time_t get_end_time();
	void populate(Record &R);
	Record record_edit(unsigned Y,unsigned X,int &r);
	int record_edit(unsigned Y,unsigned X,Record &R);
	int driver(WINDOW *wrec);
	WINDOW* mk_win(unsigned Y,unsigned X,int rnum);
	void un_mk_win(WINDOW *wrec);
};
void mvprintw(unsigned Y,unsigned X,Record &t);
int prompt_rnum();
int valid_str(char *s);
int getAfileName(char *s);
int show_results(sqlite3_stmt* s);

#endif
