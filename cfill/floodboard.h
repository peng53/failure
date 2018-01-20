#ifndef FLOODBOARD
#define FLOODBOARD
#include <string>
#include <iostream>
using std::string;
using std::ostream;

struct FloodBoard {
	struct IMPL;
	IMPL* M;
	//vector<char> b;
	unsigned rs;
	unsigned rc;
	~FloodBoard();
	FloodBoard(unsigned width,unsigned height);
	FloodBoard(char* const carr,unsigned width,unsigned height);
	void set_arr(char* const carr);
	void bshuf_normal(string& sym);
	void bshuf_normal(const char* const sym,size_t sym_l);
	void bshuf_perfect(string& sym);
	void bshuf_perfect(const char* const sym,size_t sym_l);
	int fl_fill_sl(char newcol);
	int fl_fill_q(char newcol);
};
ostream& operator<<(ostream& out, FloodBoard& f);
#endif
