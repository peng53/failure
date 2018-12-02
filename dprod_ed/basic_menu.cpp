#include <curses.h>
#include <menu.h>

int main(){
	ITEM* op[4];
	op[0] = new_item("Load Database","Load a database for editing.");
	op[1] = new_item("New Database","Create a new database for editing.");
	op[2] = new_item("Exit Program","Exit the program.");
	op[3] = 0;
	MENU* mm = new_menu(op);
	initscr();
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	attron(COLOR_PAIR(1));
	post_menu(mm);
	//attroff(COLOR_PAIR(1));
	int ch;
	int r = 1;
	int c;
	while (r==1){
		ch = getch();
		switch (ch){
			case KEY_DOWN:
				menu_driver(mm,REQ_DOWN_ITEM); break;
			case KEY_UP:
				menu_driver(mm,REQ_UP_ITEM); break;
			case 10: //< ENTER
				c = item_index(current_item(mm));
				r = 0;
				break;
		}
	}
	unpost_menu(mm);
	free_menu(mm);
	for (unsigned i=0;i<3;++i){
		free_item(op[i]);
	}
	printw("You chose item %d",c);
	getch();
	endwin();
	return 0;
}
