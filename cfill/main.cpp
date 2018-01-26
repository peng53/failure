//#include <iostream>
#include "testpatterns.cpp"
#include "floodboard.h"
#include "nout.h"

using std::cout;

int main(int argc,char** argv){
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
