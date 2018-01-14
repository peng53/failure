#include <array>
#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
#include <utility>

using namespace std;
template<typename T, size_t N>
int scanlinefill(array<T,N> &B,char col,unsigned rowl){
	char t = B[0];
	if (t==col) return 0;
	unsigned b = 0;
	//while(b<rowl && B[b]==t) ++b; // seek to end of first line
	//--b; // last part is not part of line or not same color
	while (b+1<rowl && B[b+1]==t) ++b; // sek to end of first line.
	queue<unsigned> q;
	q.emplace(0);
	q.emplace(b);
	unsigned a;
	int ta, tb, ba, bb;
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
		tb = bb = -1; // reset scanned top/bottom.
		while (a<=b){
			B[a] = col; // change current cell.
			// check top. if possible.
			if (a>=rowl && B[a-rowl]==t){
				if (tb+1==a-rowl){
					// expand >>.
					tb++;
				} else {
					if (tb!=-1){
						// there was a break. push contents and start new line.
						// first expand line >> maxward.
						//for (unsigned rb=((tb+rowl)/rowl)*rowl-1;tb+1<rb && B[tb+1]==t; ++tb){
						for (unsigned rb=((tb+rowl)/rowl)*rowl;tb+1<rb && B[tb+1]==t; ++tb){
							continue;
						}
						// post.
						q.emplace(ta);
						q.emplace(tb);
						// order is always (a,b).
					}
					// start new line.
					ta = tb = a-rowl;
					// remember top.
					// expand top.
					for (unsigned lb=(ta/rowl)*rowl;lb<ta-1 && B[ta-1]==t;--ta){
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
						for (unsigned rb=((bb+rowl)/rowl)*rowl;bb+1<rb && B[bb+1]==t; ++bb){
							continue;
						}
						// post.
						q.emplace(ba);
						q.emplace(bb);
						// order is always (a,b).
					}
					// start new line.
					ba = bb = a+rowl;
					// remember bottom.
					// expand bottom.
					for (unsigned lb=(ba/rowl)*rowl;lb<ba-1 && B[ba-1]==t;--ba){
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
			for (unsigned rb=((tb+rowl)/rowl)*rowl;tb+1<rb && B[tb+1]==t; ++tb){
				continue;
			}
			q.emplace(ta);
			q.emplace(tb);
		}
		if (bb!=-1){
			//for (unsigned rb=((bb+rowl)/rowl)*rowl-1;bb+1<rb && B[bb+1]==t; ++bb){
			for (unsigned rb=((bb+rowl)/rowl)*rowl;bb+1<rb && B[bb+1]==t; ++bb){
				continue;
			}
			q.emplace(ba);
			q.emplace(bb);
		}
	}
	return 0;
}
int sfill(array<char,100> &B,char col,unsigned rowl){
	char t = B[0];
	if (t==col) return 0;
	unsigned b = 0;
	while (b<rowl && B[b]==t){
		++b;
	}
	queue<pair<unsigned,unsigned>> q;
	q.emplace(0,b);
	unsigned ta, tb, ba, bb;
	//tb = bb = -1;
	unsigned a;
	while (!q.empty()){
		a = q.front().first;
		b = q.front().second;
		q.pop();
		if (B[a]==col){
			continue;
		}
		cout << "Got: " << a << ',' << b << '\n';
		tb = bb = -1;
		while (a<=b || b%rowl<rowl && B[b]==t){
			cout << a << '\n';
			B[a] = col;
			if (a>=rowl && B[a-rowl]==t){ //has north & target col
				if (tb==-1){ // not detected yet
					ta = tb = a-rowl;
				} else if (tb+1==a-rowl){ // detected adj line
					tb++;
				} else { // detected break in line
					// post previous line to Q and store new line.
					q.emplace(ta,tb);
					cout << "Top:" << ta << ',' << tb << '\n';
					ta = tb = a-rowl;
				}
			}
			if (a+rowl<B.size() && B[a+rowl]==t){ //has south & target col
				if (bb==-1){
					ba = bb = a+rowl;
				} else if (bb+1==a+rowl){
					bb++;
				} else {
					q.emplace(ba,bb);
					cout << "Bottom:" << ba << ',' << bb << '\n';
					ba = bb = a+rowl;
				}
			}
			a++;
			if (tb!=-1 && q.front().first!=ta && q.front().second!=tb){
				cout << a << ',' << b << ",Add1 " << ta << ',' << tb << '\n';
				q.emplace(ta,tb);
			}
			if (bb!=-1 && q.front().first!=ba && q.front().second!=bb){
				cout << a << ',' << b << ",Add2 " << ba << ',' << bb << '\n';
				q.emplace(ba,bb);
			}
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
	// a 10x10 board
	array<char,100> board;
	//tile_fill(board,6);
	/*
	size_t s = 0;
	for (auto cc : board){
		cout << cc;
		s++;
		if (s%20==0){
			cout << '\n';
		}
	}
	*/

	// using 4 colors
	size_t s = 0;
	for (char c='1';c<'5';c++){
		fill(board.begin()+s,board.begin()+s+25,c);
		s+=25;
	}
	s = 0;
	auto rng = default_random_engine {};
	shuffle(board.begin(),board.end(),rng);
	// print as 5x20
	for (auto cc : board){
		cout << cc;
		s++;
		if (s%20==0){
			cout << '\n';
		}
	}
	//char currentc = *board.begin();
	char c = '1';
	size_t changed;
	while (c!='q'){
		cin >> c;
		//changed = sfill(board,c,20);
		changed = scanlinefill(board,c,20);
		for (auto cc : board){
			cout << cc;
			s++;
			if (s%20==0){
				cout << '\n';
			}
		}
		s = 0;
		//cout << changed << '\n';
		if (changed==99){
			cout << "Done.\n";
			break;
		}
	}

	return 0;
}
