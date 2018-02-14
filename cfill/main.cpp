//#include <iostream>
#include "testpatterns.cpp"
#include "floodboard.h"
#include "nout.h"

using std::cout;

void jcout(unsigned a,unsigned b,int color){
	cout << a << ',' << b << ',' << color << '\n';
}

int main(int argc,char** argv){
	unsigned h = 6;
	unsigned w = 5;
	unsigned c_cnt = 2;
	FloodBoard f(h,w);
	string sym;
	reseed();
	f.bshuf_normal(abc_sym(sym,c_cnt));
	//oput::start();
	//oput::setup(w,h,c_cnt);
	//oput::draw_board(f.data());
	//oput::colrs(sym.c_str(),c_cnt);
	int m;
	cout << f;
	do {
		//m = oput::get_move();
		std::cin >> m;
		if (m>=0 && m<c_cnt){
			//f.fl_fill_sl(sym[m],oput::draw_scan);
			f.fl_fill_sl(sym[m],jcout);
		}
		cout << f;
	} while (m!=-1);
	//oput::end();
	/*
	FloodBoard f(5,5);
	f.set_arr(tpats::p1);
	oput::start();
	oput::setup(5,5,2);
	oput::draw_board(f.data());
	f.fl_fill_sl('x',oput::draw_scan);
	oput::get_input();
	f.fl_fill_sl('o',oput::draw_scan);
	//oput::end();

	oput::get_input();
	oput::end();
	*/
	//char t = 0;
	//std::cin >> t;
	//if (t=='1') oput::end();
	/*
	cout << "p1\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p1 done\n" << f << '\n';
	f.set_arr(tpats::p2);
	cout << "p2\n" << f;
	f.fl_fill_q('o');
	f.fl_fill_q('x');
	cout << "p2 done\n" << f << '\n';
	f.set_arr(tpats::p3);
	cout << "p3\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p3 done\n" << f << '\n';
	f.set_arr(tpats::p4);
	cout << "p4\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p4 done\n" << f << '\n';
	*/
	return 0;
}
