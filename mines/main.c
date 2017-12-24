#include <stdlib.h> // for rand in shuffleR()
#include <string.h> // for memset in mine_field()
#include <stdbool.h> // to use bool in c
#include <math.h> // for sqrt in main()
#include <time.h> // for time in nrand()
#include <curses.h> // for ui

#ifdef UPDCURSES
#define getmouse nc_getmouse
#endif


static unsigned H = 9;
static unsigned W = 9;
static unsigned mines = 9;
static unsigned Yoff = 0;
static unsigned Xoff = 0;
static char* adj = NULL;
static int* nmines = NULL;

static void shuffleR(){
	/* A Fisher-Yates like shuffle.
	 * Shuffles the nmines array.
	 */
	srand(time(NULL));
	int t;
	for (size_t i=H*W-1,j; i>0; --i){
		j = rand()%i;
		t = nmines[j];
		nmines[j] = nmines[i];
		nmines[i] = t;
	}
}

static void mine_field(int safe){
	/* Set adj up with mines where 'safe' is
	 * guaranteed to be mine-free.
	 */
	memset(adj,0,H*W); // Clears adj
	shuffleR();
	bool tw,te;
	int n;
	move(0,0); //debug
	for (int* m_ptr=nmines;m_ptr!=nmines+mines;++m_ptr){
		//< Place mines and update neighbor's nearby count.
		n = *m_ptr;
		if (n==safe){ n = nmines[H*W-1]; }
		printw("%3d",n); //debug
		adj[n] = 9; // A mine at index n.
		tw = (n%W)!=0; // Is n NOT on the left edge?
		te = (n%W)!=W-1; // Is n NOT on the right edge?
		if (tw && adj[n-1]!=9) adj[n-1]++; // If n's left neighbor is not a mine, increase left's count.
		if (te && adj[n+1]!=9) adj[n+1]++; // If n's right neighbor is not a mine, increase right's count.
		if (n>W){ // has upper neighbors.
			if (adj[n-W]!=9) adj[n-W]++; // exact north
			if (tw && adj[n-W-1]!=9) adj[n-W-1]++; // northwest neighbor
			if (te && adj[n-W+1]!=9) adj[n-W+1]++; // northeast ...
		}
		if (n<(W*(H-1))){ // has lower neighbors.
			if (adj[n+W]!=9) ++adj[n+W]; // exact south
			if (tw && adj[n+W-1]!=9) ++adj[n+W-1]; // southwest
			if (te && adj[n+W+1]!=9) ++adj[n+W+1]; // southeast
		}
	}
}

static void put_t(unsigned Y,unsigned X,int c){
	/* Prints c at Y,X and color.
	 */
	int clr = (c-'0')/2+1;
	if (c=='9') c = '*';
	else if (c=='0') c = ACS_CKBOARD;
	mvaddch(Y+Yoff,X+Xoff,c|COLOR_PAIR(clr));
}

#ifdef ONLYNZERO
static int rfil(unsigned Y,unsigned X,int left){
	// Will only recurse on empty tiles.
	if (left<1) return left;
	char* c = &adj[Y*W+X];
	if ((*c)!=0) return left;
	(*c) += 48;
	put_t(Y,X,'0');
	--left;
	if (left<1) return left;
	if (X>0) left = rfil(Y,X-1,left);
	if (X+1<W) left = rfil(Y,X+1,left);
	if (Y>0){
		left = rfil(Y-1,X,left);
		if (X>0) left = rfil(Y-1,X-1,left);
		if (X+1<W) left = rfil(Y-1,X+1,left);
	}
	if (Y+1<H){
		left = rfil(Y+1,X,left);
		if (X>0) left = rfil(Y+1,X-1,left);
		if (X+1<W) left = rfil(Y+1,X+1,left);
	}
	return left;
}
static int picked(unsigned yx[2],int left){
	// Reveals tile if not empty otherwise call rfil.
	char* c = &adj[(yx[0]*W)+yx[1]];
	if ((*c)>9) return left; // Already revealed
	if ((*c)!=0){
		(*c) += 48;
		put_t(yx[0],yx[1],(*c));
		if ((*c)=='9'){ return -1; }
		return --left;
	}
	return rfil(yx[0],yx[1],left);
}
#else
static int rfil(unsigned Y,unsigned X,int left){
	/* Reveals tile and recurses on neighbors
	 * if only if tile(x,y) is zero.
	 */
	if (left<1) return left;
	char* c = &adj[Y*W+X];
	if ((*c)>=9) return left;
	(*c) += 48;
	put_t(Y,X,*c);
	//if (*c=='9') return -1;
	--left;
	if (left<1) return left;
	if ((*c)!='0') return left;
	if (X>0) left = rfil(Y,X-1,left);
	if (X+1<W) left = rfil(Y,X+1,left);
	if (Y>0){
		left = rfil(Y-1,X,left);
		if (X>0) left = rfil(Y-1,X-1,left);
		if (X+1<W) left = rfil(Y-1,X+1,left);
	}
	if (Y+1<H){
		left = rfil(Y+1,X,left);
		if (X>0) left = rfil(Y+1,X-1,left);
		if (X+1<W) left = rfil(Y+1,X+1,left);
	}
	return left;
}
int picked(unsigned yx[2],int left){
	/* Reveals adjacent non-mine tiles. (rfil)
	 */
	if (adj[(yx[0]*W)+yx[1]]==9){
		put_t(yx[0],yx[1],'9');
		return -1;
	}
	return rfil(yx[0],yx[1],left);
}
#endif

static int get_yx(unsigned yx[2]){
	/* Keeps track of YX position until input is to
	 * be sent. (enter/q/mouse-click)
	 */
	MEVENT ev;
	int R = 0;
	while (R==0){
		mvprintw(4,0,"%05d",yx[0]*W+yx[1]);
		move(yx[0]+Yoff,yx[1]+Xoff);
		refresh();
		switch (getch()){
			case KEY_MOUSE:
				if (getmouse(&ev)==OK){
					if (ev.y>=Yoff && ev.y-Yoff<H && ev.x>=Xoff && ev.x-Xoff<W){
						yx[0] = ev.y-Yoff;
						yx[1] = ev.x-Xoff;
						R = 1;
					}
				}
				break;
			case KEY_RIGHT:
				if (yx[1]+1<W) ++yx[1];
				break;
			case KEY_LEFT:
				if (yx[1]>0) --yx[1];
				break;
			case KEY_DOWN:
				if (yx[0]+1<H) ++yx[0];
				break;
			case KEY_UP:
				if (yx[0]>0) --yx[0];
				break;
			case 10: R = 1; break;
			case 'q': R = 2; break;
		}
	}
	return R;
}
static int arrow_hand(unsigned yx[2],int left){
	int r;
	while (left>0){
		r = get_yx(yx);
		if (r==2) return -2;
		if (r==1) left = picked(yx,left);
	}
	return left;
}

static void wdriver(){
	int left;
	unsigned yx[2] = {0,0};
	while (1){
		init_pair(1,COLOR_CYAN,COLOR_BLACK);
		for (left=0;left<H;++left){ mvhline(left+Yoff,Xoff,ACS_CKBOARD,W); }
		if (get_yx(yx)!=1) break; // Bad input first pick?
		//mvprintw(1,0,"%10d",(yx[0])*W+yx[1]);
		mine_field((yx[0])*W+yx[1]);
		left = arrow_hand(yx,picked(yx,H*W-mines));
		if (left==-2) break; // User quits.
		attron(A_STANDOUT);
		mvprintw(H/2,0,"You've ");
		if (left==0){
			printw("won.");
			init_pair(1,COLOR_GREEN,COLOR_BLACK);
		} else {
			printw("found a mine.");
			init_pair(1,COLOR_RED,COLOR_BLACK);
		}
		mvprintw(H/2+1,0,"Play again? Y/N");
		attroff(A_STANDOUT);
		if (getch()=='n') break; // End game.
	}
}

int main(int argc,char** argv){
	H = (argc<2) ? 9 : atoi(argv[1]);
	W = (argc<3) ? H : atoi(argv[2]);
	mines = (argc<4) ? (unsigned)sqrt(H*W) : atoi(argv[3]);
	adj = (char*)malloc(H*W*sizeof(char));
	if (adj==NULL){ return 1; }
	nmines = (int*)malloc(H*W*sizeof(int));
	if (nmines==NULL){
		free(adj);
		return 1;
	}
	for (size_t i=0;i<H*W;++i){ nmines[i] = i; }
	initscr();
	start_color();
	Yoff = (LINES-H)/2;
	Xoff = (COLS-W)/2;
	//init_pair(1,COLOR_CYAN,COLOR_BLACK);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_RED,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	cbreak();
	noecho();
	keypad(stdscr,1);
	mousemask(BUTTON1_PRESSED,NULL);
	wdriver();
	endwin();
	free(adj);
	free(nmines);
	return 0;
}
