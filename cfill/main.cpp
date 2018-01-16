#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
#include <string>
#include <cstring>
#include "testpatterns.cpp"


using namespace std;

static auto rng = default_random_engine {};

void rscanline(char *B,unsigned width,size_t bsize,char newcol,unsigned a,unsigned b){
	// Recolors scanline a,b to newcol.
	// The oldcol is derived from the first point of the inital scanline.
	queue<unsigned> q;
	q.emplace(a);
	q.emplace(b);
	int u1, u2, l1, l2;
	char oldcol = B[a];
	u2 = l2 = -1;
	while (!q.empty()){
		a = q.front();
		q.pop();
		b = q.front();
		q.pop();
		if (B[a]==newcol){
			cout << "(REPEAT) ";
			continue;
		}
		cout << "G(" << a << ',' << b << ") ";
		for (;a<=b;++a){
			B[a] = newcol;
			if (a>width && B[a-width]==oldcol){
				if (u2+1==a-width){ u2++; }
				else {
					if (u2!=-1){
						q.emplace(u1);
						q.emplace(u2);
					}
					u1 = u2 = a-width;
					for (unsigned lb=u1-u1%width;u1-1>=lb && B[u1-1]==oldcol; --u1){ continue; }
				}
			}
			if (a+width<bsize && B[a+width]==oldcol){
				if (l2+1==a+width){ l2++; }
				else {
					if (l2!=-1){
						q.emplace(l1);
						q.emplace(l2);
					}
					l1 = l2 = a+width;
					for (unsigned lb=l1-l1%width;l1-1>=lb && B[l1-1]==oldcol; --l1){ continue; }
				}
			}
		}
		if (u2!=-1){
			for (unsigned rb=u2+width-(u2%width)-1;u2+1<=rb && B[u2+1]==oldcol; ++u2){ continue; }
			q.emplace(u1);
			q.emplace(u2);
			u1 = u2 = -1;
		}
		if (l2!=-1){
			for (unsigned rb=l2+width-(l2%width)-1;l2+1<=rb && B[l2+1]==oldcol; ++l2){ continue; }
			q.emplace(l1);
			q.emplace(l2);
			l1 = l2 = -1;
		}
	}
	cout << '\n';
}

int scanlinefill(char* board,unsigned width,unsigned height,char newchar){
	// Calls a scanline flood fill on the first scanline of the board.
	// The function mainly expands the default scanline from 0,0 to its
	// max. To make a similar call on some arbitrary (in-bound), use
	// rscanline with arguments board,width,size,replacement-color,scanline-a,scanline-b
	// scanline-b should be expanded prior to this call.
	char oldchar = board[0];
	if (newchar==oldchar) return 0;
	unsigned b = 0;
	while (b+1<width && board[b+1]==oldchar) ++b; // seek to end of first line.
	rscanline(board,width,width*height,newchar,0,b);
	return 0;
}

int r_qfill(char* B,unsigned width,size_t bsize,char newcol,unsigned pt){
	queue<unsigned> Q;
	char oldcol = B[pt];
	if (oldcol==newcol) return 0;
	Q.push(pt);
	unsigned w, e;
	size_t changed = 1;
	while (!Q.empty()){
		w = e = Q.front();
		Q.pop();
		while (w%width>0 && B[w]==oldcol){
			w--;
		}
		while ((e%width)<(width-1) && B[e]==oldcol){
			e++;
		}
		for (;w<=e;++w){
			B[w]=newcol;
			if (w>=width && B[w-width]==oldcol){
				Q.push(w-width);
				changed++;
			}
			if (w+width<bsize && B[w+width]==oldcol){
				Q.push(w+width);
				changed++;
			}
		}
	}
	return changed;
}

int qfill(char* board,unsigned width,unsigned height,char newcol){
	// Calls flood fill (with queue) on the top left corner with newcol(er)
	// To make similar call on arbitrary point (in-bound) replace 0 with that pt.
	return r_qfill(board,width,width*height,newcol,0);
}

template<size_t N>
struct CFloodBoard {
	char b[N];
	unsigned rs; // row-size (width)
	unsigned rc; // row-count
	CFloodBoard(unsigned width,unsigned height): rs(width), rc(height){}
	void set_arr(char* const carr){
		strncpy(b,carr,N);
	}
	void bshuf_normal(const char*  const sym,size_t sym_l){
		// TBD
		normal_distribution<> r{((double)rs*rc)/sym_l,sym_l*0.02};
		unsigned t;
		size_t siz = 0;
		for (size_t i=1;i<sym_l;++i){
			t = round(r(rng));
			memset(b+siz,sym[i],t);
			siz+=t;
		}
		if (b+siz<b+rs*rc){
			memset(b+siz,sym[0],rs*rc-siz);
		}
		b[rs*rc]='\0';
		shuffle(b,b+rs*rc,rng);
	}

};
template<typename T, size_t N>
struct FloodBoard {
	array<T,N> b;
	unsigned rs;
	unsigned rc;
	FloodBoard(unsigned width,unsigned height,string& sym):rs(width), rc(height){
		b = array<T,N>();
		bshuf_normal(sym);
	}
	FloodBoard(T* const carr,unsigned width,unsigned height): rs(width){
		copy(carr,carr+width*height,b.begin());
	}
	void set_arr(T* const carr,unsigned height){
		copy(carr,carr+rs*height,b.begin());
	}
	void bshuf_normal(string& sym){
		//< sym must have atleast 2 characters to be meaningful.
		normal_distribution<> r{(double)b.size()/sym.length(),sym.length()*0.02};
		unsigned t;
		auto it = b.begin();
		for (size_t i=1;i<sym.length();++i){
			t = round(r(rng));
			fill(it,it+t,sym[i]);
			it+=t;
		}
		if (it<b.end()){
			fill(it,b.end(),sym.front());
		}
		shuffle(b.begin(),b.end(),rng);
	}
	void bshuf_perfect(string& sym){
		unsigned d = b.size()/sym.length();
		auto it = b.begin();
		for (char c : sym){
			fill(it,it+d,c);
			it+=d;
		}
		fill(it,b.end(),sym.back());
		shuffle(b.begin(),b.end(),rng);
	}
	int fl_fill_sl(char newcol){
		// Calls scanlinefill with newcol.
		// This by default selects first scanline as
		// the oldcol.
		return scanlinefill(b.data(),rs,rc,newcol);
	}
	int fl_fill_q(char newcol){
		return qfill(b.data(),rs,rc,newcol);
	}
};
template<typename T, size_t N>
ostream& operator<<(ostream& out,FloodBoard<T,N>& f){
	unsigned i = 0;
	for (auto c : f.b){
		out << c;
		++i;
		if (i==f.rs){
			i = 0;
			out << '\n';
		}
	}
	return out;
}

int main(int argc,char** argv){
	FloodBoard<char,25> f(tpats::p1,5,5);
	cout << "p1\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p1 done\n" << f << '\n';
	f.set_arr(tpats::p2,5);
	cout << "p2\n" << f;
	f.fl_fill_q('o');
	f.fl_fill_q('x');
	cout << "p2 done\n" << f << '\n';
	f.set_arr(tpats::p3,5);
	cout << "p3\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p3 done\n" << f << '\n';
	f.set_arr(tpats::p4,5);
	cout << "p4\n" << f;
	f.fl_fill_q('x');
	f.fl_fill_q('X');
	cout << "p4 done\n" << f << '\n';
	CFloodBoard<26> g(5,5);
	const char* sym = "abc";
	g.bshuf_normal(sym,3);
	cout << g.b << ".\n";
	return 0;
}
