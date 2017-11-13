// dprod_cursed.cpp

#include <ncurses.h>
#include "rec.cpp"

int build_a_record(){
	char s[80];
	WINDOW* a_record = newwin(LINES-1,COLS>>1,0,COLS>>1);
	wattron(a_record,A_BOLD);
	mvwprintw(a_record,1,2,"Record Creation");
	wattroff(a_record,A_BOLD);
	mvwprintw(a_record,3,2,"UID");
	wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	mvwaddch(a_record,4,2,'[');
	mvwaddch(a_record,4,13,']');
	wattroff(a_record,COLOR_PAIR(1));
	mvwprintw(a_record,6,2,"Start Date");
	mvwprintw(a_record,7,3,"YYYY  MM  DD  -  hh : mm");
	wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	mvwprintw(a_record,8,2,"[    ][  ][  ]   [  ] [  ]");
	wattroff(a_record,COLOR_PAIR(1));

	mvwprintw(a_record,10,2,"End Date");
	mvwprintw(a_record,11,3,"YYYY  MM  DD  -  hh : mm");
	wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	mvwprintw(a_record,12,2,"[    ][  ][  ]   [  ] [  ]");
	wattroff(a_record,COLOR_PAIR(1));

	mvwprintw(a_record,14,2,"Code");
	wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	mvwaddch(a_record,15,2,'[');
	mvwaddch(a_record,15,13,']');
	wattroff(a_record,COLOR_PAIR(1));

	box(a_record,0,0);
	refresh();
	wrefresh(a_record);
	wmove(a_record,4,3); //uid
	wgetnstr(a_record,s,10);
	wmove(a_record,8,3); //d1yr
	wgetnstr(a_record,s,4);
	wmove(a_record,8,9); //d1mth
	wgetnstr(a_record,s,2);
	wmove(a_record,8,13); //d1d
	wgetnstr(a_record,s,2);
	wmove(a_record,8,20); //d1hh
	wgetnstr(a_record,s,2);
	wmove(a_record,8,25); //d1mm
	wgetnstr(a_record,s,2);
	wmove(a_record,12,3); //d2yr
	wgetnstr(a_record,s,4);
	wmove(a_record,12,9); //d2mth
	wgetnstr(a_record,s,2);
	wmove(a_record,12,13); //d2d
	wgetnstr(a_record,s,2);
	wmove(a_record,12,20); //d2hh
	wgetnstr(a_record,s,2);
	wmove(a_record,12,25); //d2mm
	wgetnstr(a_record,s,2);
	wmove(a_record,15,3); //uid
	wgetnstr(a_record,s,10);
	delwin(a_record);
	refresh();
	return 0;
}