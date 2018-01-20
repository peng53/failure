#include <iostream>
#include "testpatterns.cpp"
#include "floodboard.h"

using std::cout;

int main(int argc,char** argv){
	FloodBoard f(5,5);
	f.set_arr(tpats::p1);
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
	return 0;
}
