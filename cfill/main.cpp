#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
//#include <cstdlib>
#include <string>
#include "testpatterns.cpp"

using namespace std;

static auto rng = default_random_engine {};

template<typename T, size_t N>
struct FloodBoard {
	array<T,N> b;
	unsigned rs;
	FloodBoard(unsigned width,string& sym):rs(width){
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
		for (unsigned i=1;i<sym.length();++i){
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
	int b = 0;
	while (b+1<rowl && B[b+1]==t) ++b; // seek to end of first line.
	queue<int> q;
	q.emplace(0);
	q.emplace(b);
	int a;
	int u1, u2, l1, l2;
	u2 = l2 = -1;
	while (!q.empty()){
		a = q.front();
		q.pop();
		b = q.front();
		q.pop();
		if (B[a]==col){
			// got a repeat.
			cout << "(REPEAT) ";
			continue; // already done
		}
		cout << "G(" << a << ',' << b << ") ";
		//while (a<=b){
		for (;a<=b;++a){
			B[a] = col;
			if (a>rowl && B[a-rowl]==t){
				if (u2==-1){
					u1 = u2 = a-rowl;
					for (unsigned lb=u1-u1%rowl;u1-1>=lb && B[u1-1]==t; --u1){ continue; }
				} else {
					if (u2+1==a-rowl){
						u2++;
					} else {
						//for (unsigned rb=(u2/rowl)*rowl+rowl-1;u2+1<=rb && B[u2+1]==t; ++u2){ continue; }
						//for (unsigned rb=(u2/rowl)*rowl+rowl-1;u2+1<=rb && B[u2+1]==t; ++u2){ continue; }
						q.emplace(u1);
						q.emplace(u2);
						u1 = u2 = a-rowl;
						//cout << u1 << ',' << u2 << '\n';
					}
				}
			}
			if (a+rowl<B.size() && B[a+rowl]==t){
				if (l2==-1){
					l1 = l2 = a+rowl;
					for (unsigned lb=l1-l1%rowl;l1-1>=lb && B[l1-1]==t; --l1){ continue; }
				} else {
					if (l2+1==a+rowl){
						l2++;
					} else {
						//for (unsigned rb=(l2/rowl)*rowl+rowl-1;l2+1<=rb && B[l2+1]==t; ++l2){ continue; }
						//cout << l1 << ',' << l2 << '\n';
						q.emplace(l1);
						q.emplace(l2);
						l1 = l2 = a+rowl;
					}
				}
			}
			//++a;
		}
		if (u2!=-1){
			for (unsigned rb=(u2/rowl)*rowl+rowl-1;u2+1<=rb && B[u2+1]==t; ++u2){ continue; }
			q.emplace(u1);
			q.emplace(u2);
			u1 = u2 = -1;
		}
		if (l2!=-1){
			for (unsigned rb=(l2/rowl)*rowl+rowl-1;l2+1<=rb && B[l2+1]==t; ++l2){ continue; }
			q.emplace(l1);
			q.emplace(l2);
			l1 = l2 = -1;
		}
	}
	cout << '\n';
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

int main(int argc,char** argv){
	FloodBoard<char,25> f(tpats::p1,5,5);
	cout << f;
	scanlinefill(f.b,'x',f.rs);
	scanlinefill(f.b,'X',f.rs);
	cout << f << '\n';
	f.set_arr(tpats::p2,5);
	cout << f;
	scanlinefill(f.b,'x',f.rs);
	scanlinefill(f.b,'X',f.rs);
	cout << f << '\n';
	f.set_arr(tpats::p3,5);
	cout << f;
	scanlinefill(f.b,'x',f.rs);
	scanlinefill(f.b,'X',f.rs);
	cout << f;

	return 0;
}
