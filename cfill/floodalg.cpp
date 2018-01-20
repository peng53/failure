#include <queue>
#include "floodalg.h"
#ifdef INC_IOSTREAM
#define DEBUG(x) do { std::cerr << x << '\n'; } while (0)
#else
#define DEBUG(x)
#endif

using std::queue;
void rscanline(char *B,unsigned width,size_t bsize,char newcol,unsigned a,unsigned b){
	// Recolors scanline a,b to newcol.
	// The oldcol is derived from the first point of the inital scanline.
	//
	queue<unsigned> q;
	q.emplace(a);
	q.emplace(b);
	int u1, u2, l1, l2;
	char oldcol = B[a];
	u2 = l2 = -1;
	while (!q.empty()){
		// The complexity of this loop should be
		// 3*w*n + r*w.
		// where w=width, n=# of scanlines, r=repeated scanlines.
		// A repeat looks like:
		// xxxxxxx   (line 1)
		// xxxoxxx   <-- starting scanline. (line3 a & b)
		// xxxxxxx   (line 2)
		// xxxxxxx   (line 4)
		// This while loop will begin coloring line2 before line1
		// this will add line3b (and line4) to the queue
		// while coloring line1, line3b will be found (& detected) once again
		// and added to the queue. When we get to line3b initally, no lines will
		// be added. The second time line3b will be detected as already done.
		// the w (in r*w) is for the time used in redetecting the scanline.
		// Perhaps the scanlines can be hashed by their first point?
		// This would be trading space for time, but for a FloodTable of cells C
		// there should be an upper limit on # of scanlines.
		// Note that n (# of scanlines) is dependent on the height and that
		// reshaping the FloodTable will decrease w but increase n.
		// I have not tested for 'balance points' on this.
		a = q.front();
		q.pop();
		b = q.front();
		q.pop();
		if (B[a]==newcol){
			DEBUG("(REPEAT) ");
			continue;
		}
		DEBUG("G(" << a << ',' << b << ") ");
		for (;a<=b;++a){
			// The complexity of this loop is b-a+1 which is the current scanline's length
			// plus length of lines above and below
			// So max time should be where |current_scanline|==|below|==|above|==width.
			// Where current scanline is neither the top or bottom one.
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
	DEBUG('\n');
}

int scanlinefill(char* board,unsigned width,unsigned height,char newchar){
	// Calls a scanline flood fill on the first scanline of the board.
	// The function mainly expands the default scanline from 0,0 to its
	// max. To make a similar call on some arbitrary (in-bound), use
	// rscanline with arguments board,width,size,replacement-color,scanline-a,scanline-b
	// scanline-b should be expanded prior to this call.

	// scanlinefill complexity is based on length of first scanline
	// plus the call to rscanline.
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
