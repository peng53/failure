//#include <stdio.h>
#include <stdlib.h> // for rand in nrand()
#include <string.h> // for memset in mine_field()
#include <stdbool.h>
#include <math.h> // for sqrt in main()
#include <time.h> // for time in nrand()
#include <ncurses.h>

int* shuffle(int* A,size_t s){
	srand(time(NULL));
	size_t j;
	int t;
	//short j,t;
	for (size_t i=s-1; i>0; --i){
		j = rand()%i;
		t = A[j];
		A[j] = A[i];
		A[i] = t;
	}
	return A;
}
int* nrange(int n){
	int* A = (int*)malloc(n*sizeof(int));
	if (!A) return 0;
	while (--n>=0){
		A[n] = n;
	}
	return A;
}
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
	for (size_t j;s>0;--s){ //< Shuffling
		j = rand()%s;
		t = A[j];
		A[j] = A[s];
		A[s] = t;
	}
	return A;
}

//short* mine_field(int H,int W,int mines){
char* mine_field(unsigned H,unsigned W,unsigned mines){
	/**
	 * Given Height, Width, and mines: creates a dynamically
	 * allocated array whose values are the # of mines near it.
	 * A mine is labeled as '*'
	*/
	int* nmines = nrand(H*W); //< Get mine positions.
	if (!nmines) return 0; //< Malloc fail of nmines.
	char* adjm = (char*)malloc(H*W*sizeof(char));
	memset(adjm,'0',H*W);
	//< Create empty array (0 mines)
	if (!adjm){  //< Malloc fail.
		free(nmines);
		return 0;
	}
	bool tw,te;
	for (int n;mines>0;--mines){
		//< Place mines are update neighbor's nearby count.
		n = nmines[mines];
		adjm[n] = '*'; // A mine at index n.
		tw = (n%W)!=0; // Is n NOT on the left edge?
		te = (n%W)!=W-1; // Is n NOT on the right edge?
		if (tw && adjm[n-1]!='*') adjm[n-1]++; // If n's left neighbor is not a mine, increase left's count.
		if (te && adjm[n+1]!='*') adjm[n+1]++; // If n's right neighbor is not a mine, increase right's count.
		if (n>W){ // has upper neighbors.
			if (adjm[n-W]!='*') adjm[n-W]++; // exact north
			if (tw && adjm[n-W-1]!='*') adjm[n-W-1]++; // northwest neighbor
			if (te && adjm[n-W+1]!='*') adjm[n-W+1]++; // northeast ...
		}
		if (n<(W*(H-1))){ // has lower neighbors.
			if (adjm[n+W]!='*') ++adjm[n+W]; // exact south
			if (tw && adjm[n+W-1]!='*') ++adjm[n+W-1]; // southwest
			if (te && adjm[n+W+1]!='*') ++adjm[n+W+1]; // southeast
		}
	}
	free(nmines); // Served its purpose.
	return adjm;
}

void playerview(unsigned Yoff,unsigned Xoff,char* adj, bool* rv,unsigned W,size_t s){
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	unsigned col=0;
	int clr;
	for (size_t i=0;i<s;++i){
		if (col==W){
			++Yoff;
			col = 0;
		}

		if (rv[i]){
			if (adj[i]!='0'){
				switch (adj[i]){
					case '*':
					case '8':
						clr = 9;
						break;
					case '7':
					case '6':
					case '5':
						clr = 8;
						break;

					case '4':
					case '3':
						clr = 7;
						break;
					case '2':
					case '1':
						clr = 6;
						break;
				}
				mvaddch(Yoff,Xoff+col,adj[i]|COLOR_PAIR(clr));
			}
		}
		++col;
	}
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));
}

int picked(unsigned Xcoord,unsigned Ycoord);

int main(int argc,char** argv){
	unsigned H = (argc<2) ? 9 : atoi(argv[1]);
	unsigned W = (argc<3) ? H : atoi(argv[2]);
	unsigned mines = (argc<4) ? (unsigned)sqrt(H*W) : atoi(argv[3]);
	char* adj = mine_field(H,W,mines);
	if (!adj){ // Failure in malloc.
		return 1;
	}
	bool* known = (bool*)calloc(H*W,sizeof(bool));

	if (!known){ // Failed to allocate known.
		free(adj);
		return 1;
	}
	for (size_t i=0;i<H*W;++i){
		known[i] = 1;
	}
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(6,COLOR_WHITE,COLOR_BLACK);
	init_pair(7,COLOR_YELLOW,COLOR_BLACK);
	init_pair(8,COLOR_RED,COLOR_BLACK);
	init_pair(9,COLOR_MAGENTA,COLOR_BLACK);
	//bkgd(COLOR_PAIR(1));
	/*
	unsigned row=0;
	unsigned col=0;
	for (size_t i=0;i<H*W;++i){
		addch(adj[i]);
		col++;
		if (col==W){
			col=0;
			++row;
			move(row,0);
		}
	}*/
	/*
	attron(COLOR_PAIR(2));
	mvvline(0,0,' ',H+2);
	mvvline(0,W+1,' ',H+2);
	mvhline(0,1,' ',W);
	mvhline(H+1,1,' ',W);
	attroff(COLOR_PAIR(2));
	*/
	playerview(1,1,adj,known,W,W*H);
	refresh();
	getch();
	endwin();
	free(known);
	free(adj);
	return 0;
}
