#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

std::vector<char> nfield(std::vector<short> &mines,short hw,short t,short m){
	for (short i=0;i<t;++i) mines[i]=i;
	srand(time(NULL));
	std::random_shuffle(mines.begin(),mines.end());
	mines.resize(m);
	std::vector<char> aj(t,0);
	bool tw,te;
	short n;
	for (short i=0;i<m;++i){
		n=mines[i];
		aj[n]=-1;
		tw=(n%hw)!=0;
		te=(n%hw)!=hw-1;
		if (tw && aj[n-1]!=-1) ++aj[n-1];
		if (te && aj[n+1]!=-1) ++aj[n+1];
		if (n>hw){
			if (aj[n-hw]!=-1) ++aj[n-hw];
			if (tw && aj[n-hw-1]!=-1) ++aj[n-hw-1];
			if (te && aj[n-hw+1]!=-1) ++aj[n-hw+1];
		}
		if (n<(hw*(hw-1))){
			if (aj[n+hw]!=-1) ++aj[n+hw];
			if (tw && aj[n+hw-1]!=-1) ++aj[n+hw-1];
			if (te && aj[n+hw+1]!=-1) ++aj[n+hw+1];
		}
	}
	return aj;
}
void printfus(std::vector<char> &aj,short hw,short t){
	putchar('/');
	for (short i=0;i<hw;i++) printf("%d",i%10);
	putchar('+');
	for (short i=0;i<hw;i++) printf("%d",i%10);
	for (short i=0;i<t;i+=hw){
		printf("\n%d",(i/hw)%10);
		for (short j=i;((j==i) || (j%hw!=0)); ++j) putchar((aj[j]==-1) ? '*' : ' ');
		putchar('|');
		for (short j=i;((j==i) || (j%hw!=0)); ++j){
			switch (aj[j]){
				case 0: putchar(' '); break;
				case -1: putchar('*'); break;
				default: printf("%d",aj[j]); break;
			}
		}
	}
	putchar('\n');
}
void printrf(std::vector<char> &rf, short hw, short t,short left){
	putchar('/');
	for (short i=0;i<hw;i++) printf("%d",i%10);
	printf("->left:%d",left);
	for (short i=0;i<t;++i){
		if (i%hw==0) printf("\n%d",(i/hw)%10);
		switch (rf[i]){
			case -2: putchar('.'); break;
			case -1: putchar('*'); break;
			case 0: putchar(' '); break;
			default: printf("%d",rf[i]); break;
		}
	}
	putchar('\n');
}
short xyn(short x,short y,short hw){
	return (hw*y)+x;
}
void rfil(std::vector<char> &aj,std::vector<char> &rf, short n,short hw,short &l){
	if (rf[n]!=-2) return; // no change
	if (aj[n]!=0){
		if (aj[n]!=-1){
			rf[n]=aj[n];
			l--;
		}
		return;
	}
	l--;
	rf[n]=0;
	bool tw=(n%hw)!=0;
	bool te=(n%hw)!=hw-1;
	if (tw) rfil(aj,rf,n-1,hw,l);
	if (te) rfil(aj,rf,n+1,hw,l);
	if (n>hw){
		rfil(aj,rf,n-hw,hw,l);
		if (tw) rfil(aj,rf,n-hw-1,hw,l);
		if (te) rfil(aj,rf,n-hw+1,hw,l);
	}
	if (n<(hw)*(hw-1)){
		rfil(aj,rf,n+hw,hw,l);
		if (tw) rfil(aj,rf,n+hw-1,hw,l);
		if (te) rfil(aj,rf,n+hw+1,hw,l);
	}
}
void pickc(std::vector<char> &aj,std::vector<char> &rf, short n,short hw,short &l){
	if (rf[n]!=-2) return;
	if (aj[n]==-1){
		rf[n]=-1;
		l=-1;
		return;
	}
	rfil(aj,rf,n,hw,l);
}
void revil(std::vector<char> &rf, std::vector<short> &mines,short m){
	for (short i=0;i<m;++i) rf[mines[i]]=-1;
}
int main(int argc,char *argv[]){
	short hw = (argc>1) ? atoi(argv[1]) : 10;
	short m = (argc>2) ? atoi(argv[2]) : 10;
	if (hw<1 || m<1) return 1;
	short t = hw*hw;
	if (t<m) return 1;
	short x,y;
	short left = t-m;
	std::vector<short> mines(t);
	std::vector<char> aj = nfield(mines,hw,t,m);
	printfus(aj,hw,t);
	std::vector<char> rf(t,-2);
	printrf(rf,hw,t,left);
	while (left>=1){
		printf("Enter x,y: ");
		scanf("%hd,%hd",&x,&y);
		if (x<0 || y<0 || x>hw || y>hw) continue;
		pickc(aj,rf,xyn(x,y,hw),hw,left);
		printrf(rf,hw,t,left);
	}
	if (left!=0){
		revil(rf,mines,m);
		printrf(rf,hw,t,left);
	}
	printf("you %s\n",(left==0) ? "win" : "lose");
	return 0;
}
