#ifndef D_UI
#define D_UI
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <vector>
#include "db.h"

class nRecord {
	private:
		FORM* F;
		FIELD* f[15];
		int rnum;
		void dress_rec_win(WINDOW* W);
		time_t get_start_time();
		time_t get_end_time();
		int driver(WINDOW *wrec);
	public:
		nRecord();
		~nRecord();
		WINDOW* view(const unsigned Y,const unsigned X);
		void un_view(WINDOW *wrec);
		int edit(const unsigned Y,const unsigned X);
		void populate(const Record &R);
		void depopulate();
		Record exportr();
		Record& exportr(Record &R);
};
class mainMenu {
	private:
		ITEM *op[4];
		MENU *M;
	public:
		mainMenu();
		~mainMenu();
		int run();
};
int init_viewr();
int nloop();
int cleanup();
int getAfileName(char *s);
void database_mnip(const unsigned Y,const unsigned X,const unsigned l,SQLi &db);

namespace FilterForm {
	void init();
	void clean();
	void dress();
	void show();
	int driver();
	void loop();
	void hide();
}

#endif
