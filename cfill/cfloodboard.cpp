#include "floodalg.h"
#include "floodboard.h"
#include <algorithm>
#include <random>
#include <cstring>
using std::shuffle;

auto rng = std::default_random_engine {};
void reseed(int s){
	rng.seed(s);
}
void reseed(){
	rng.seed(std::random_device {}());
}
struct FloodBoard::IMPL {
	char* b;
	IMPL(size_t s){
		b = new char[s];
	}
	~IMPL(){
		delete b;
	}
};
FloodBoard::FloodBoard(unsigned width,unsigned height): rs(width), rc(height){
	M = new IMPL(width*height);
}
FloodBoard::~FloodBoard(){
	delete M;
}

void FloodBoard::set_arr(char* const carr){
	strncpy(M->b,carr,rc*rs);
}
void FloodBoard::bshuf_normal(const char* const sym,size_t sym_l){
	// TBD
	std::normal_distribution<> r{((double)rs*rc)/sym_l,sym_l*0.02};
	unsigned t;
	size_t siz = 0;
	for (size_t i=1;i<sym_l;++i){
		t = round(r(rng));
		memset((M->b)+siz,sym[i],t);
		siz+=t;
	}
	if ((M->b)+siz<(M->b)+rs*rc){
		memset((M->b)+siz,sym[0],rs*rc-siz);
	}
	(M->b)[rs*rc]='\0';
	shuffle((M->b),(M->b)+rs*rc,rng);
}
void FloodBoard::bshuf_perfect(const char* const sym,size_t sym_l){
	// not entirely correct.
	unsigned d = (rs*rc)/sym_l;
	size_t siz = 0;
	for (size_t i=0;i<sym_l;++i){
		memset((M->b)+siz,sym[i],d);
		siz+=d;
	}
	if ((M->b)+siz<(M->b)+rs*rc){
		memset((M->b)+siz,sym[sym_l],rs*rc-siz);
	}
	(M->b)[rs*rc]='\0';
	shuffle((M->b),(M->b)+rs*rc,rng);
}
int FloodBoard::fl_fill_sl(char newcol, std::function<void (unsigned,unsigned,int)> snitch){
	// Calls scanlinefill with newcol.
	// This by default selects first scanline as
	// the oldcol.
	return scanlinefill_new(M->b,rs,rc,newcol,snitch);
}
int FloodBoard::fl_fill_q(char newcol, std::function<void (unsigned,unsigned,int)> snitch){
	return qfill(M->b,rs,rc,newcol,snitch);
}
char* FloodBoard::data(){
	return M->b;
}
char* abc_sym(char* s,size_t len){
	// s should be large enough to hold the characters plus the terminator
	for (size_t i=0;i<len;i++){
		s[i] = ('a'+i);
	}
	return s;
}

ostream& operator<<(ostream& out,FloodBoard& f){
	unsigned i=0;
	for (size_t s = 0;s<f.rs*f.rc;++s){
		out << f.M->b[s] << out.fill();
		++i;
		if (i==f.rs){
			i = 0;
			out << '\n';
		}
	}
	return out;
}
