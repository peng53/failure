#include <cstdio>
#include <cstring>
#include <ncurses.h>
#include "ui.cpp"

int main(){
	//~ char s[] = "AAAAAAAAAAAA";
	//~ fgets(s,10,stdin);
	//~ size_t n = 0;
	//~ int r = valid_str(s,10,n);
	//~ printf("%d\n",r);
	//~ for (int i=0;i<n;++i){
		//~ putchar(s[i]);
	//~ }
	TRecord t;
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	int r = build_a_record(0,0,t);
	refresh();
	getch();
	endwin();
	printf("%d\n",r);
	if (r==0){
		t.qfix();
		t.print();
	}
	return 0;
}