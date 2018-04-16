#include <ncurses.h>
#include "mprint.h"

namespace MazePrint {
	void print_cell(int cell,int y,int x){
		// where y,x is top left corner
		mvaddch(y,x,ACS_CKBOARD);
		if (!((cell>>3) & 1)) mvaddch(y,x+1,ACS_CKBOARD);
		mvaddch(y,x+2,ACS_CKBOARD);
		// middle.
		if (!((cell>>0)& 1)) mvaddch(y+1,x,ACS_CKBOARD);
		mvaddch(y+1,x+1,cell ? cell+'0' : ACS_CKBOARD);
		if (!((cell>>2) & 1)) mvaddch(y+1,x+2,ACS_CKBOARD);

		mvaddch(y+2,x,ACS_CKBOARD);
		if (!((cell>>1) & 1)) mvaddch(y+2,x+1,ACS_CKBOARD);
		mvaddch(y+2,x+2,ACS_CKBOARD);
	}
}
