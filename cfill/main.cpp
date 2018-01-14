#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
#include <string>
#include "testpatterns.cpp"

using namespace std;

template<typename T, size_t N>
struct FloodBoard {
	array<T,N> b;
	unsigned rs;
	FloodBoard(unsigned width,string& sym):rs(width){
		b = array<T,N>();
		bshuf(sym);
	}
	void bshuf(string& sym){
		unsigned d = b.size()/sym.length();
		auto it = b.begin();
		for (char c : sym){
			fill(it,it+d,c);
			it+=d;
		}
		fill(it,b.end(),sym.back());
		bshuf();
	}
	void bshuf(){
		auto rng = default_random_engine {};
		shuffle(b.begin(),b.end(),rng);
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

template<typename T, size_t N>
int scanlinefill(array<T,N> &B,char col,unsigned rowl){
	char t = B[0];
	if (t==col) return 0;
	unsigned b = 0;
	//while(b<rowl && B[b]==t) ++b; // seek to end of first line
	//--b; // last part is not part of line or not same color
	while (b+1<rowl && B[b+1]==t) ++b; // seek to end of first line.
	queue<unsigned> q;
	q.emplace(0);
	q.emplace(b);
	unsigned a;
	int ta, tb, ba, bb;
	tb = bb = -1;
	while (!q.empty()){
		a = q.front();
		q.pop();
		b = q.front();
		q.pop();
		cout << "Got: " << a << ',' << b << '\n';
		if (B[a]==col){
			// got a repeat.
			cout << "REPEAT\n";
			continue; // already done
		}
		//tb = bb = -1; // reset scanned top/bottom.
		while (a<=b){
			B[a] = col; // change current cell.
			// check top. if possible.
			if (a>=rowl && B[a-rowl]==t){
				if (tb+1==a-rowl){ // continue top line
					tb++;
				} else {
					if (tb!=-1){
						// previous line ended.
						//for (int rb=tb+rowl-1-(tb%rowl);tb+1<=rb && B[tb+1]==t; ++tb){
						//	continue;
						//}
						// post.
						q.emplace(ta);
						q.emplace(tb);
					}
					// start new line and then expand it.
					ta = tb = a-rowl;
					for (int lb=ta-(ta%rowl);lb<=ta-1;--ta){
						continue;
					}
				}
			}
			// now do the same for bottom. if possible.
			if (a+rowl<B.size() && B[a+rowl]==t){
				if (bb+1==a+rowl){
					// expand >>.
					bb++;
				} else {
					if (bb!=-1){
						// there was a break. push contents and start new line.
						// first expand line >> maxward.
						//for (unsigned rb=((bb+rowl)/rowl)*rowl-1;bb+1<rb && B[bb+1]==t; ++bb){
						//for (int rb=bb+rowl-1-(bb%rowl);bb+1<=rb && B[bb+1]==t; ++bb){
						//	continue;
						//}
						// post.
						q.emplace(ba);
						q.emplace(bb);
						// order is always (a,b).
					}
					// start new line.
					ba = bb = a+rowl;
					// remember bottom.
					// expand bottom.
					for (int lb=ta-(ta%rowl);lb<=ta-1;--ta){
						continue;
					}
				}
			}
			// checked top and bottom at this point.
			// proceed on current line.
			a++;
		}
		// post lines if not already posted.
		if (tb!=-1){
			// first expand them.
			for (int rb=tb+rowl-1-(tb%rowl);tb+1<=rb && B[tb+1]==t; ++tb){
				continue;
			}
			q.emplace(ta);
			q.emplace(tb);
			tb = -1;
		}
		if (bb!=-1){
			//for (unsigned rb=((bb+rowl)/rowl)*rowl-1;bb+1<rb && B[bb+1]==t; ++bb){
			for (int rb=bb+rowl-1-(bb%rowl);tb+1<=bb && B[bb+1]==t; ++bb){
				continue;
			}
			q.emplace(ba);
			q.emplace(bb);
			bb = -1;
		}
	}
	return 0;
}


int bfill(array<char,100> &B,char col,unsigned rowl){
	queue<unsigned> Q;
	char t = B[0];
	if (t==col) return 0;
	Q.push(0);
	unsigned w, e;
	size_t changed = 1;
	while (!Q.empty()){
		w = e = Q.front();
		Q.pop();
		while (w%rowl>0 && B[w]==t){
			w--;
		}
		while ((e%rowl)<(rowl-1) && B[e]==t){
			e++;
		}
		for (;w<=e;++w){
			B[w]=col;
			if (w>=rowl && B[w-rowl]==t){
				Q.push(w-rowl);
				changed++;
			}
			//if (w<=(B.size()-rowl) && B[w]==t){
			if (w+rowl<B.size() && B[w+rowl]==t){
				Q.push(w+rowl);
				changed++;
			}
		}
	}
	return changed;
}

array<char,100> gen_board(unsigned colors,unsigned per){
	array<char,100> board;
	char c='a';
	for (unsigned i=0;i<colors;++i){
		fill(board.begin()+i*per,board.begin()+(i+1)*per,c);
		c++;
	}
	return board;
}

template<typename T, size_t N>
void tile_fill(array<T,N> &board,unsigned colors){
	normal_distribution<> r{(double)board.size()/colors,board.size()*0.02};
	unsigned count = 0;
	unsigned i;
	auto rng = default_random_engine {};
	colors--;
	cout << r.min() << '\n';
	cout << r.max() << '\n';
	while (colors>0){
		i = round(r(rng));
		cout << i << '\n';
		//fill(board.begin()+count,board.begin()+count+i,'a'+colors);
		count = i;
		--colors;
	}
}

int main(){
	string f_col = "abc";
	FloodBoard<char,100> f(20,f_col);
	cout << f;
	scanlinefill(f.b,'c',f.rs);
	scanlinefill(f.b,'a',f.rs);
	scanlinefill(f.b,'x',f.rs);
	cout << f;

	return 0;
}
