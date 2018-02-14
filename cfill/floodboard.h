#ifndef FLOODBOARD
#define FLOODBOARD
#include <string>
#include <iostream>
#include <functional>
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
	int fl_fill_sl(char newcol, std::function<void (unsigned,unsigned,int)> snitch);
	int fl_fill_q(char newcol, std::function<void (unsigned,unsigned,int)> snitch);
	char* data();
};
char* abc_sym(char* s,size_t len);
string& abc_sym(string& s,size_t len);
ostream& operator<<(ostream& out, FloodBoard& f);
void reseed(int);
void reseed();
#endif
