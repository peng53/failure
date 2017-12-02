#ifndef D_UI
#define D_UI
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <map>
#include "db.h"

struct nRecord {
	FORM* F;
	FIELD* f[15];
	int rnum;
	nRecord();
	~nRecord();
	void dress_rec_win(WINDOW* W);
	time_t get_start_time();
	time_t get_end_time();
	WINDOW* view(unsigned Y,unsigned X);
	void un_view(WINDOW *wrec);
	int edit(unsigned Y,unsigned X);
	int driver(WINDOW *wrec);
	void populate(const Record &R);
	void depopulate();
	Record exportr();
	int exportr(Record &R);
};

struct mainMenu {
	ITEM *op[11];
	MENU *M;
	std::map<char,int> keys;
	mainMenu();
	~mainMenu();
	void more_ops(bool i);
	int run();
	int has_op(char);
	int has_op_sc(char ch);

};
void mvprintw(unsigned Y,unsigned X,const Record &t);
int prompt_rnum();
int valid_str(char *s);
int getAfileName(char *s);
int show_results(sqlite3_stmt* s);

#endif
