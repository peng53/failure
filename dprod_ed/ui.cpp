#ifndef D_UI
#define D_UI
#include "db.cpp"
#include "ui.h"
#include <cstdlib>
#include <ncurses.h>

int valid_str(char *s){
	/**
	Checks if string has length and counts length.
	Return codes:
	0: success
	1: no/bad input
	2: special input
	*/
	if (s[0]=='\0' || s[0]=='\n' && s[1]=='\0'){ return 1; }
	if (s[0]=='\\'){ return 2; }
	return 0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t M){
	int r = 1;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		r = valid_str(s);
	}
	return r==0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char* s,size_t M,unsigned int &n){
	int r = 1;
	char *end;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		if (s[0]=='\\'){ r=1; }
		n = (unsigned int)strtoul(s,&end,10);
		if (!*end){ r=0; }
	}
	return r==0;
}
void record_creation_win(WINDOW* W){
	/**
	Prints parts of the record creation window.
	*/
	wattron(W,A_BOLD);
	mvwprintw(W,1,2,"Record Creation");
	wattroff(W,A_BOLD);
	/// the fields
	wattron(W,COLOR_PAIR(1));
	mvwhline(W,3,6,ACS_BLOCK,10);
	mvwhline(W,3,23,ACS_BLOCK,5);
	mvwhline(W,7,3,ACS_BLOCK,16);
	mvwhline(W,11,3,ACS_BLOCK,16);
	mvwhline(W,14,3,ACS_BLOCK,25);
	mvwhline(W,15,3,ACS_BLOCK,25);
	mvwhline(W,16,3,ACS_BLOCK,25);
	wattroff(W,COLOR_PAIR(1));
	/// labels above the fields
	mvwprintw(W,3,2,"UID");
	mvwprintw(W,3,18,"Code");
	mvwprintw(W,5,2,"Start Date\n   MM/DD/YYYY hh:mm");
	mvwprintw(W,9,2,"End Date\n   MM/DD/YYYY hh:mm");
	mvwprintw(W,13,2,"Description");
	mvwaddch(W,7,5,'/');
	mvwaddch(W,7,8,'/');
	mvwaddch(W,7,13,' ');
	mvwaddch(W,7,16,':');
	mvwaddch(W,11,5,'/');
	mvwaddch(W,11,8,'/');
	mvwaddch(W,11,13,' ');
	mvwaddch(W,11,16,':');
	box(W,0,0);
	wrefresh(W);
}
int build_a_record(unsigned short Y,unsigned short X,Record &T){
	WINDOW* a_record = newwin(20,31,Y,X);
	record_creation_win(a_record);
	wattron(a_record,A_REVERSE);
	char uid[11] = "\0";
	char code[6] = "\0";
	char s[4];
	unsigned int date[10];
	if (!valid_wrap(a_record,3,6,uid,10) ||
		!valid_wrap(a_record,3,23,code,5) ||
		!valid_wrap(a_record,7,3,s,2,date[0]) ||
		!valid_wrap(a_record,7,6,s,2,date[1]) ||
		!valid_wrap(a_record,7,9,s,4,date[2]) ||
		!valid_wrap(a_record,7,14,s,2,date[3]) ||
		!valid_wrap(a_record,7,17,s,2,date[4]) ||
		!valid_wrap(a_record,11,3,s,2,date[5]) ||
		!valid_wrap(a_record,11,6,s,2,date[6]) ||
		!valid_wrap(a_record,11,9,s,4,date[7]) ||
		!valid_wrap(a_record,11,14,s,2,date[8]) ||
		!valid_wrap(a_record,11,17,s,2,date[9])
	){ return 1; }
	T.uid = string(uid,10);
	T.code = string(code,5);
	T.ds = to_time_t(date);
	T.de = to_time_t(date+5);
	char desc[26] = "\0";
	mvwgetnstr(a_record,14,3,desc,25); ///< desc-l1
	int r = valid_str(desc);
	if (r==2){ return 1; }
	if (r!=1){
		T.desc = string(desc,25);
		mvwgetnstr(a_record,15,3,desc,25); ///< desc-l2
		r = valid_str(desc);
		if (r==2){ return 1; }
		if (r!=1){
			T.append_desc(desc);
			mvwgetnstr(a_record,16,3,desc,25); ///< desc-l3
			r = valid_str(desc);
			if (r==2){ return 1; }
			if (r!=1){ T.append_desc(desc); }
		}
	}
	wattroff(a_record,A_REVERSE);
	wclear(a_record);
	delwin(a_record);
	return 0;
}
int getAfileName(char *s){
	int r = 1;
	while (r==1){
		mvprintw(LINES-1,0,"FILE:");
		mvgetnstr(LINES-1,5,s,99);
		r = valid_str(s);
		if (r==2){ return 2; }
	}
	return 0;
}
int show_results(sqlite3_stmt* s){
	struct tm *t;
	time_t d;
	for (int n = 1; sqlite3_step(s)==100 && n<25; ++n){
		mvprintw(n,0,"%02d %-10s",n,sqlite3_column_text(s,0));
		d = sqlite3_column_int(s,1);
		t = gmtime(&d);
		mvprintw(n,14,"%02d/%02d/%04d %02d:%02d",t->tm_mon+1,t->tm_mday,t->tm_year+1900,t->tm_hour,t->tm_min);
		d = sqlite3_column_int(s,2);
		t = gmtime(&d);
		mvprintw(n,31,"%02d/%02d/%04d %02d:%02d",t->tm_mon+1,t->tm_mday,t->tm_year+1900,t->tm_hour,t->tm_min);
		mvprintw(n,46,"%-5d",sqlite3_column_int(s,3));
	}
	mvvline(0,13,ACS_VLINE,LINES-1);
	mvvline(0,30,ACS_VLINE,LINES-1);
	return 0;
}

#endif