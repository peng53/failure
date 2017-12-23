#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N_MINES 20
#define N_WIDTH 9
#define N_CELLS 100
#define T_CELLS short

void rmine(T_CELLS *mines){
	srand(time(NULL));
	T_CELLS j,t;
	for (T_CELLS i=N_CELLS-1;i>0;--i){
		j=rand()%i;
		t=mines[j];
		mines[j]=mines[i];
		mines[i]=t;
	}
}
void nfields(char *aj,T_CELLS *mines){
	bool tw,te;
	short n;
	for (T_CELLS i=N_MINES;i>0;--i){
		n=mines[i];
		aj[n]=-1;
		tw=(n%N_WIDTH)!=0;
		te=(n%N_WIDTH)!=N_WIDTH-1;
		if (tw && aj[n-1]!=-1) ++aj[n-1];
		if (te && aj[n+1]!=-1) ++aj[n+1];
		if (n>N_WIDTH){
			if (aj[n-N_WIDTH]!=-1) ++aj[n-N_WIDTH];
			if (tw && aj[n-N_WIDTH-1]!=-1) ++aj[n-N_WIDTH-1];
			if (te && aj[n-N_WIDTH+1]!=-1) ++aj[n-N_WIDTH+1];
		}
		if (n<(N_WIDTH*(N_WIDTH-1))){
			if (aj[n+N_WIDTH]!=-1) ++aj[n+N_WIDTH];
			if (tw && aj[n+N_WIDTH-1]!=-1) ++aj[n+N_WIDTH-1];
			if (te && aj[n+N_WIDTH+1]!=-1) ++aj[n+N_WIDTH+1];
		}
	}
}
void printfus(char *aj){
	for (T_CELLS i=0;i<N_CELLS;++i){
		if (i%N_WIDTH==0) putchar('\n');
		printf("(%2d)",i);
		switch (aj[i]){
			case -1: putchar('*');break;
			case 0: putchar(' ');break;
			default: printf("%d",aj[i]); break;
		}
	}
	putchar('\n');
}
void printfus2(char *aj){
	putchar('/');
	for (short i=0;i<N_WIDTH;++i) printf("%d",i%10);
	putchar('+');
	for (short i=0;i<N_WIDTH;++i) printf("%d",i%10);
	for (short i=0;i<N_CELLS;i+=N_WIDTH){
		printf("\n%d",(i/N_WIDTH)%10);
		for (short j=i;((j==i) || (j%N_WIDTH!=0)); ++j) putchar((aj[j]==-1) ? '*' : ' ');
		putchar('|');
		for (short j=i;((j==i) || (j%N_WIDTH!=0)); ++j){
			switch (aj[j]){
				case 0: putchar(' '); break;
				case -1: putchar('*'); break;
				default: printf("%d",aj[j]); break;
			}
		}
	}
	putchar('\n');
}
T_CELLS get_r(T_CELLS n){
	return n/N_WIDTH;
}
T_CELLS get_c(T_CELLS n){
	return n%N_WIDTH;
}
void printrf(bool *rf,char *aj){
	printf("%2s|"," ");
	for (T_CELLS i=0;i<N_WIDTH;++i) printf("%3d",i);
	for (T_CELLS i=0;i<N_CELLS;++i){
		if (i%N_WIDTH==0) printf("\n%2d|",i/N_WIDTH);
		if (rf[i]){
			switch (aj[i]){
				case -1: printf("%3s","*"); break;
				case 0: printf("%3s"," "); break;
				default: printf("%3d",aj[i]); break;
			}
		} else printf("%3s","+");
	}
	putchar('\n');
}
T_CELLS rfil(char *aj,bool *rf,T_CELLS n,T_CELLS left){
	if (rf[n]) return left;
	if (aj[n]==-1) return left;
	rf[n]=1;
	/*
	--left;
	if (aj[n]!=0) return left;
	*/
	if (aj[n]!=0) return left-1;
	bool tw=(n%N_WIDTH)!=0;
	bool te=(n%N_WIDTH)!=N_WIDTH-1;
	if (tw) left=rfil(aj,rf,n-1,left);
	if (te) left=rfil(aj,rf,n+1,left);
	if (n>N_WIDTH){
		left=rfil(aj,rf,n-N_WIDTH,left);
		if (tw) left=rfil(aj,rf,n-N_WIDTH-1,left);
		if (te) left=rfil(aj,rf,n-N_WIDTH+1,left);
	}
	if (n<(N_WIDTH)*(N_WIDTH-1)){
		left=rfil(aj,rf,n+N_WIDTH,left);
		if (tw) left=rfil(aj,rf,n+N_WIDTH-1,left);
		if (te) left=rfil(aj,rf,n+N_WIDTH+1,left);
	}
	//return left;
	return --left;
}
T_CELLS pickc(char *aj,bool *rf,T_CELLS n,T_CELLS left){
	//printf("left start at %d\n",left);
	if (rf[n]) return left;
	if (aj[n]==-1){
		rf[n]=1;
		return -1;
	}
	return rfil(aj,rf,n,left);
}
int main(){
	//multigame
	T_CELLS left,n;
	T_CELLS mines[N_CELLS];
	char aj[N_CELLS];
	bool rf[N_CELLS];
	while (1){
		left = N_CELLS - N_MINES;
		for (T_CELLS i=N_CELLS-1;i>=0;--i){
			aj[i]=rf[i]=0;
			mines[i]=i;
		}
		rmine(mines);
		nfields(aj,mines);
		printfus2(aj);
		while (left>0){
			printrf(rf,aj);
			scanf("%hu",&n);
			if (n<0 || n>=N_CELLS) continue;
			left=pickc(aj,rf,n,left);
		}
		//printf("%d\n",left);
		printrf(rf,aj);
	}
	return 0;
}
