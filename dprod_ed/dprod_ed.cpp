#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include "dprod_cursed.cpp"

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	build_a_record();
	refresh();

	getch();
	endwin();
	return 0;
}