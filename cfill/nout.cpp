#include <ncurses.h>
#include <cstdlib>
#include "nout.h"

namespace oput {
	unsigned yoff = 0, xoff = 0, height = 0, width = 0, colors = 1;
	void start(){
		initscr();
		start_color();
		init_pair(0,COLOR_WHITE,COLOR_BLACK);
		init_pair(1,COLOR_RED,COLOR_BLACK);
		init_pair(2,COLOR_GREEN,COLOR_BLACK);
		init_pair(3,COLOR_YELLOW,COLOR_BLACK);
		init_pair(4,COLOR_BLUE,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(6,COLOR_CYAN,COLOR_BLACK);
		init_pair(9,COLOR_WHITE,COLOR_BLACK);
		noecho();
		raw();
	}
	void end(){
		endwin();
	}
	void setup(unsigned w,unsigned h,unsigned c){
		width = w;
		height = h;
		colors = c;
		yoff = (LINES-h)/2;
		xoff = (COLS-w)/2;
	}
	void draw_scan(unsigned a,unsigned b,int color){
		if (b%width<a%width){
			printw("scanline is too long!");
		}
		//printw("i was called");
		attron(COLOR_PAIR(color%colors));
		mvhline(yoff+a/width,xoff+(a%width),ACS_BOARD,b-a+1);
		//mvaddch(yoff+n/width,xoff+(n%width),ACS_BOARD | COLOR_PAIR(color));
		attroff(COLOR_PAIR(color%colors));
		mvprintw(yoff+a/width,0,"%d to %d",a,b);
		//move(0,0);
		//printw("l @ %d : %d w/ %d",a/width,(a%width),b-a+1);
		//refresh();
		//getch();
	}
	void draw_scan_y(unsigned y,unsigned x0,unsigned x1,int color){
		//attron(COLOR_PAIR(color%colors));
		//mvhline(yoff+y,xoff+x0,color,x1-x0+1);
		mvhline(yoff+y,xoff+x0,color | COLOR_PAIR(color%colors) ,x1-x0+1);
		move(0,0);
		//attroff(COLOR_PAIR(color%colors));
	}
	void draw_board(char* data){
		unsigned l = 0;
		for (size_t i = 0, s = width*height; i < s;){
			move(yoff+l,xoff);
			for (unsigned c = 0; c < width; ++c){
				//addch(ACS_BOARD | COLOR_PAIR(data[i]%colors));
				addch(data[i] | COLOR_PAIR(data[i]%colors));
				i++;
			}
			mvprintw(l+yoff,width+1+xoff+10,"%d",(1+l)*width-1);
			l++;
		}
		printw("\nwidth: %u; height: %u",width,height);
		refresh();
	}
	void delay(){
		//printw("something new");
		refresh();
		getch();
	}
	int get_input(){
		char t[10];
		return getnstr(t,0);
	}
	int get_move(){
		char t[5];
		getnstr(t,3);
		if (t[0]<'0' || t[0]>'9') return -1;
		return atoi(t);
	}
	void colrs(const char* cs,size_t len){
		addch('\n');
		for (size_t i=0;i<len;++i){
			addch('0'+i | COLOR_PAIR(i));
		}
	}
	void ps(char* s){
		addstr(s);
	}
}
