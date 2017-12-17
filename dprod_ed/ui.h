#ifndef D_UI
#define D_UI
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <vector>
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
	Record& exportr(Record &R);
};
struct mainMenu {
	ITEM *op[4];
	MENU *M;
	mainMenu();
	~mainMenu();
	int run();
};

int getAfileName(char *s);
void database_mnip(unsigned Y,unsigned X,unsigned l,SQLi &db);

#endif
