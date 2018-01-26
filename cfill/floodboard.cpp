#include "floodalg.h"
#include "floodboard.h"
#include <algorithm>
#include <random>
#include <vector>
using std::shuffle;

auto rng = std::default_random_engine {};

struct FloodBoard::IMPL {
	std::vector<char> b;
};

FloodBoard::FloodBoard(unsigned width,unsigned height):rs(width), rc(height){
	M = new IMPL;
	M->b.resize(width*height);
	//bshuf_normal(sym);
}
FloodBoard::~FloodBoard(){
	delete M;
}
FloodBoard::FloodBoard(char* const carr,unsigned width,unsigned height): rs(width), rc(height){
	M = new IMPL;
	M->b.resize(width*height);
	std::copy(carr,carr+width*height,M->b.begin());
}
void FloodBoard::set_arr(char* const carr){
	std::copy(carr,carr+rs*rc,(M->b).begin());
}
void FloodBoard::bshuf_normal(string& sym){
	//< sym must have atleast 2 characters to be meaningful.
	std::normal_distribution<> r{(double)(M->b.size())/sym.length(),sym.length()*0.02};
	unsigned t;
	auto it = M->b.begin();
	for (size_t i=1;i<sym.length();++i){
		t = round(r(rng));
		fill(it,it+t,sym[i]);
		it+=t;
	}
	if (it<M->b.end()){
		fill(it,M->b.end(),sym.front());
	}
	shuffle(M->b.begin(),M->b.end(),rng);
}
void FloodBoard::bshuf_perfect(string& sym){
	unsigned d = M->b.size()/sym.length();
	auto it = M->b.begin();
	for (char c : sym){
		fill(it,it+d,c);
		it+=d;
	}
	fill(it,M->b.end(),sym.back());
	shuffle(M->b.begin(),M->b.end(),rng);
}
int FloodBoard::fl_fill_sl(char newcol, std::function<void (unsigned,unsigned,int)> snitch){
	// Calls scanlinefill with newcol.
	// This by default selects first scanline as
	// the oldcol.
	return scanlinefill(M->b.data(),rs,rc,newcol, snitch);
}
int FloodBoard::fl_fill_q(char newcol, std::function<void (unsigned,unsigned,int)> snitch){
	return qfill(M->b.data(),rs,rc,newcol,snitch);
}
char* FloodBoard::data(){
	return M->b.data();
}

ostream& operator<<(ostream& out,FloodBoard& f){
	unsigned i = 0;
	for (auto c : f.M->b){
		out << c;
		++i;
		if (i==f.rs){
			i = 0;
			out << '\n';
		}
	}
	return out;
}
