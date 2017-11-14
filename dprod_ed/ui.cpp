#ifndef D_UI
#define D_UI
#include "ui.h"

int build_a_record(unsigned short Y,unsigned short X){
	char s[26];
	WINDOW* a_record = newwin(20,31,Y,X);
	wattron(a_record,A_BOLD);
	mvwprintw(a_record,1,2,"Record Creation");
	wattroff(a_record,A_BOLD);
	wattron(a_record,COLOR_PAIR(1));
	mvwhline(a_record,3,6,ACS_BLOCK,10);
	mvwhline(a_record,3,23,ACS_BLOCK,5);
	mvwhline(a_record,7,3,ACS_BLOCK,16);
	mvwhline(a_record,11,3,ACS_BLOCK,16);
	mvwhline(a_record,14,3,ACS_BLOCK,25);
	mvwhline(a_record,15,3,ACS_BLOCK,25);
	mvwhline(a_record,16,3,ACS_BLOCK,25);
	wattroff(a_record,COLOR_PAIR(1));

	mvwprintw(a_record,3,2,"UID");
	mvwprintw(a_record,3,18,"Code");
	mvwprintw(a_record,5,2,"Start Date");
	mvwprintw(a_record,6,3,"MM/DD/YYYY hh:mm");
	mvwprintw(a_record,9,2,"End Date");
	mvwprintw(a_record,10,3,"MM/DD/YYYY hh:mm");
	mvwprintw(a_record,13,2,"Description");
	mvwaddch(a_record,7,5,'/');
	mvwaddch(a_record,7,8,'/');
	mvwaddch(a_record,7,13,' ');
	mvwaddch(a_record,7,16,':');
	mvwaddch(a_record,11,5,'/');
	mvwaddch(a_record,11,8,'/');
	mvwaddch(a_record,11,13,' ');
	mvwaddch(a_record,11,16,':');
	box(a_record,0,0);
	wrefresh(a_record);
	wattron(a_record,A_REVERSE);
	mvwgetnstr(a_record,3,6,s,10); //uid
	mvwgetnstr(a_record,3,23,s,5); //code
	mvwgetnstr(a_record,7,3,s,2); //d1mth
	mvwgetnstr(a_record,7,6,s,2); //d1d
	mvwgetnstr(a_record,7,9,s,4); //d1yr
	mvwgetnstr(a_record,7,14,s,2); //d1hh
	mvwgetnstr(a_record,7,17,s,2); //d1mm
	mvwgetnstr(a_record,11,3,s,2); //d2mth
	mvwgetnstr(a_record,11,6,s,2); //d2d
	mvwgetnstr(a_record,11,9,s,4); //d2yr
	mvwgetnstr(a_record,11,14,s,2); //d2hh
	mvwgetnstr(a_record,11,17,s,2); //d2mm
	mvwgetnstr(a_record,14,3,s,25); //desc-l1
	mvwgetnstr(a_record,15,3,s,25); //desc-l2
	mvwgetnstr(a_record,16,3,s,25); //desc-l3
	wattroff(a_record,A_REVERSE);
	delwin(a_record);
	return 0;
}

#endif