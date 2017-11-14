#ifndef D_UI
#define D_UI
#include "ui.h"

int build_a_record(){
	char s[80];
	WINDOW* a_record = newwin(LINES-1,COLS>>1,0,COLS>>1);
	wattron(a_record,A_BOLD);
	mvwprintw(a_record,1,2,"Record Creation");
	wattroff(a_record,A_BOLD);
	mvwprintw(a_record,3,2,"UID ");
	wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	whline(a_record,ACS_BLOCK,10);
	wattroff(a_record,COLOR_PAIR(1));
	mvwprintw(a_record,3,18,"Code ");
	wattron(a_record,COLOR_PAIR(1));
	whline(a_record,ACS_BLOCK,5);
	wattroff(a_record,COLOR_PAIR(1));
	
	mvwprintw(a_record,5,2,"Start Date");
	mvwprintw(a_record,6,3,"MM/DD/YYYY hh:mm");
	wattron(a_record,COLOR_PAIR(1));
	wmove(a_record,7,3);
	whline(a_record,ACS_BLOCK,16);
	wattroff(a_record,COLOR_PAIR(1));
	mvwaddch(a_record,7,5,'/');
	mvwaddch(a_record,7,8,'/');
	mvwaddch(a_record,7,13,' ');
	mvwaddch(a_record,7,16,':');
	//~ mvwprintw(a_record,5,2,"Start Date   ||   End Date");
	//~ mvwprintw(a_record,6,14,"YYYY");
	//~ mvwprintw(a_record,7,14,"MMDD");
	//~ mvwprintw(a_record,9,14,"hhmm");

	//~ wattron(a_record,COLOR_PAIR(1));
	//~ wmove(a_record,6,4);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,7,4);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,9,4);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,6,23);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,7,23);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,9,23);
	//~ whline(a_record,ACS_BLOCK,4);

	wattroff(a_record,COLOR_PAIR(1));
	//~ mvwprintw(a_record,5,2,"Start Date");
	//~ mvwprintw(a_record,6,3,"YYYY  MM  DD  -  hh : mm");
	//~ wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	//~ mvwprintw(a_record,7,2,"[    ][  ][  ]   [  ] [  ]");
	//~ wattroff(a_record,COLOR_PAIR(1));

	//~ mvwprintw(a_record,9,2,"End Date");
	//~ mvwprintw(a_record,10,3,"YYYY  MM  DD - hh : mm");
	//~ wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	//~ wmove(a_record,11,3);
	//~ whline(a_record,ACS_BLOCK,4);
	//~ wmove(a_record,11,9);
	//~ whline(a_record,ACS_BLOCK,2);
	//~ wmove(a_record,11,13);
	//~ whline(a_record,ACS_BLOCK,2);
	//~ wmove(a_record,11,18);
	//~ whline(a_record,ACS_BLOCK,2);
	//~ wmove(a_record,11,23);
	//~ whline(a_record,ACS_BLOCK,2);
	
	//~ //mvwprintw(a_record,11,2,"[    ][  ][  ]   [  ] [  ]");
	//~ wattroff(a_record,COLOR_PAIR(1));

	//~ mvwprintw(a_record,13,2,"Description");
	//~ wattron(a_record,COLOR_PAIR(1)); // 1-> white bg black fg
	//~ mvwaddch(a_record,14,2,'[');
	//~ mvwaddch(a_record,14,13,']');
	//~ wattroff(a_record,COLOR_PAIR(1));

	box(a_record,0,0);
	refresh();
	wrefresh(a_record);
	wattron(a_record,COLOR_PAIR(1));
	wattron(a_record,A_REVERSE);
	wmove(a_record,3,6); //uid
	wgetnstr(a_record,s,10);
	wmove(a_record,3,23); //code
	wgetnstr(a_record,s,5);
	//~ wmove(a_record,7,3); //d1yr
	//~ wgetnstr(a_record,s,4);
	//~ wmove(a_record,7,9); //d1mth
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,7,13); //d1d
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,7,20); //d1hh
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,7,25); //d1mm
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,11,3); //d2yr
	//~ wgetnstr(a_record,s,4);
	//~ wmove(a_record,11,9); //d2mth
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,11,13); //d2d
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,11,18); //d2hh
	//~ wgetnstr(a_record,s,2);
	//~ wmove(a_record,11,23); //d2mm
	//~ wgetnstr(a_record,s,2);
	//wmove(a_record,15,3); //uid
	//wgetnstr(a_record,s,10);
	wattroff(a_record,COLOR_PAIR(1));
	delwin(a_record);
	refresh();
	return 0;
}

#endif