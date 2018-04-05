//#include <iostream>
#include "testpatterns.cpp"
#include "floodboard.h"
#include "nout.h"

using std::cout;

void jcout(unsigned a,unsigned b,int color){
	cout << a << ',' << b << ',' << color << '\n';
}

int main(int argc,char** argv){
	unsigned h = 14;
	unsigned w = 10;
	unsigned c_cnt = 7;
	FloodBoard f(w,h);
	string sym;
	reseed();
	f.bshuf_normal(abc_sym(sym,c_cnt));
	int m;
	cout << f << "----\n";
	do {
		std::cin >> m;
		if (m>=0 && m<c_cnt){
			f.fl_fill_sl(sym[m],jcout);
		}
		cout << f << "----\n";
	} while (m!=-1);

	return 0;
}
