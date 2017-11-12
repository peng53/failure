#include <cstdio>
#include <cstring>
#include <ncurses.h>

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	WINDOW* mywin = newwin(LINES-1,COLS>>1,0,0);
	wattron(mywin,A_BOLD);
	wprintw(mywin,"\n Records\n");
	wattroff(mywin,A_BOLD);
	wattron(mywin,COLOR_PAIR(1));
	box(mywin,0,0);
	wattroff(mywin,COLOR_PAIR(1));
	refresh();
	wrefresh(mywin);
	WINDOW* details = newwin(LINES-1,COLS>>1,0,COLS>>1);
	wprintw(details,"\n Details");
	box(details,0,0);
	refresh();
	wrefresh(details);
	getch();
	delwin(mywin);
	delwin(details);
	endwin();
	return 0;
}