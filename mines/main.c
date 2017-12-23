#include <stdlib.h> // for rand in nrand()
#include <string.h> // for memset in mine_field()
#include <stdbool.h>
#include <math.h> // for sqrt in main()
#include <time.h> // for time in nrand()
#include <curses.h>

static unsigned H = 9;
static unsigned W = 9;
static unsigned mines = 9;
static unsigned Yoff = 1;
static unsigned Xoff = 1;
static char* adj = NULL;

int* nrand(size_t s){
	/**
	 * Given s, return a shuffled dynamically allocated int-array
	 * with ints from 0 to s-1. The shuffling uses Fisher-Yates(?).
	 */
	int* A = (int*)malloc(s*sizeof(int));
	if (!A) return 0;
	int t=s;
	while (0<=--t){ //< Filling range
		A[t] = t;
	}
	srand(time(NULL));
	--s;
	for (size_t j;s>0;--s){ //< Shuffling
		j = rand()%s;
		t = A[j];
		A[j] = A[s];
		A[s] = t;
	}
	return A;
}

void mine_field(int safe){
	int* nmines = nrand(H*W);
	if (!nmines) return;
	memset(adj,0,H*W);
	//< Create empty array (0 mines)
	bool tw,te;
	int n;
	for (int* m_ptr=nmines;m_ptr!=nmines+mines;++m_ptr){
		//< Place mines are update neighbor's nearby count.
		n = *m_ptr;
		if (n==safe){ n = nmines[H*W-1]; }
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
	free(nmines); // Served its purpose.
}

void put_t(unsigned Y,unsigned X,int c){
	int clr = (c-'0')/2+1;
	if (c=='9') c = '*';
	else if (c=='0') c = ACS_CKBOARD;
	mvaddch(Y+Yoff,X+Xoff,c|COLOR_PAIR(clr));
}

int rfil2(unsigned Y,unsigned X,int left){
	if (left<1) return left;
	char* c = &adj[Y*W+X];
	if ((*c)>9) return left;
	(*c) += 48;
	put_t(Y,X,*c);
	if (*c=='9') return -1;
	--left;
	if (*c!='0') return left;
	if (X>0) left = rfil2(Y,X-1,left);
	if (X+1<W) left = rfil2(Y,X+1,left);
	if (Y>0){
		left = rfil2(Y-1,X,left);
		if (X>0) left = rfil2(Y-1,X-1,left);
		if (X+1<W) left = rfil2(Y-1,X+1,left);
	}
	if (Y+1<H){
		left = rfil2(Y+1,X,left);
		if (X>0) left = rfil2(Y+1,X-1,left);
		if (X+1<W) left = rfil2(Y+1,X+1,left);
	}
	return left;
}
int picked2(unsigned Y,unsigned X,int left){
	return rfil2(Y-Yoff,X-Xoff,left);
}

int get_yx(unsigned* yx){
	while (1){
		move(yx[0],yx[1]);
		refresh();
		switch (getch()){
			case KEY_RIGHT:
				if ((yx[1])+1-Xoff<W) ++yx[1];
				break;
			case KEY_LEFT:
				if ((yx[1])>Xoff) --yx[1];
				break;
			case KEY_DOWN:
				if ((yx[0])+1-Yoff<H) ++yx[0];
				break;
			case KEY_UP:
				if ((yx[0])>Yoff) --yx[0];
				break;
			case 10: return 1; break;
			case 'q': return -2; break;
		}
	}
	return 0;
}
int first_pick(){
	unsigned yx[2] = {Yoff,Xoff};
	if (get_yx(yx)==1){
		return (yx[0])*W+(yx[1]);
	}
	return -1;
}
int arrow_hand(int left){
	unsigned yx[2] = {Yoff,Xoff};
	int r;
	while (left>0){
		r = get_yx(yx);
		if (r==-2) return -2;
		if (r==1) left = picked2(yx[0],yx[1],left);
	}
	return left;
}

void wdriver(){
	int left;
	int n;
	while (1){
		for (left=0;left<H;++left){ mvhline(left+Yoff,Xoff,ACS_CKBOARD,W); }
		n = first_pick();
		if (n==-1) return;
		mine_field(n-(Yoff*W)-Xoff);
		left = arrow_hand(picked2(n/W,n%W,H*W-mines));
		if (left==-2) return;
		if (left==0){ mvprintw(Yoff,Xoff,"You've won!"); }
		else { mvprintw(Yoff,Xoff,"You've found a mine."); }
		mvprintw(Yoff+1,Xoff+1,"Play again? Y/N");
		if (getch()=='n') return;
	}
}

int main(int argc,char** argv){
	H = (argc<2) ? 9 : atoi(argv[1]);
	W = (argc<3) ? H : atoi(argv[2]);
	mines = (argc<4) ? (unsigned)sqrt(H*W) : atoi(argv[3]);
	adj = (char*)malloc(H*W*sizeof(char));
	if (adj==NULL){ return 1; }
	initscr();
	start_color();
	init_pair(1,COLOR_CYAN,COLOR_BLACK);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_YELLOW,COLOR_BLACK);
	init_pair(4,COLOR_RED,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	cbreak();
	noecho();
	keypad(stdscr,1);
	wdriver();
	endwin();
	free(adj);
	return 0;
}
