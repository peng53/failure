#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include "ui.cpp"

int main(){
	TRecord t;
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	int r = build_a_record(0,0,t);
	refresh();
	getch();
	endwin();
	if (r==0){
		t.qfix();
		t.print();
	}
	return 0;
}