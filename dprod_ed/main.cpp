#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include "ui.cpp"

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	build_a_record(0,0);
	refresh();

	getch();
	endwin();
	return 0;
}