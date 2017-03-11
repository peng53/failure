#include <string>
#include <cstring>
#include <iostream>
#include "b_inc.cpp"
#include "st.cpp"

using std::cout;
using std::string;

int what_flag(const char *s){
	switch (strlen(s)){
	case 0: return 0;
	case 2:
		switch (s[1]){
		case 's': return 1;
		case 'b': return 2;
		case 'B': return 3;
		case 'c': return 4;
		}
	}
	return 9;
}
void literal_storer(char** &A,char **A_end,PartedString &P){
	for (string I;A!=A_end;++A){
		switch(what_flag(*A)){
		case 0: break;
		case 1: ++A; return;
		case 2:
			if (A+2<A_end && strlen(*(A+1))==6){
				I = b_incs(*(++A));
				P << remove_excs(I,*(++A));
				break;
			}
		case 3:
			if (A+1<A_end && strlen(*(A+1))==6){
				P << b_incs(*(++A)); break;}
		default: P << string {*A};
		}
	}
	return;
}
unsigned justnumbers(unsigned U[],unsigned m,const string &t){
	// Given a string, fill array U with up to m unsigned ints
	// Non-digits are ignored (or treated as delimiter)
	// while a sequence of digits as a number; unary minus and
	// decimal '.' are ignored as well.
	unsigned c=0, b, n;
	for (size_t i=0,l=t.length();(i<l && c<m);++c){
		for (n=1;(i<l && !isdigit(t[i]));++i) continue; // find start
		if (i>=l) return 0;
		for (b=i++;(i<l && isdigit(t[i++]));++n) continue; //count digits
		U[c] = stoul(t.substr(b,n)); // or atoi on .c_str()
	}
	return c;
}
void auto_part(const string &t, PartedString &P){
	if (t.length()==0) return;
	unsigned U[4];
	unsigned lits_s = P.lits_size(), c = justnumbers(U,4,t);
	if (c==0) P.add_part(t);
	else if (U[0]<lits_s){
		switch (c){
		case 1: P.add_part(U[0]); break;
		case 2: P.add_part(U[0],U[1]); break;
		case 3: if (U[1]<lits_s) P.add_part(U[0],U[1],U[2]); break;
		case 4: if (U[2]<lits_s) P.add_part(U[0],U[1],U[2],U[3]); break;
		}
	}
}
void dry_run(PartedString &P){
	unsigned lits = P.lits_size();
	P << string {'\n'};
	for (unsigned i=0;i<lits;++i){
		P.add_part(i);
		P.add_part(lits);
	}
}
void part_storer(char** &A, char** A_end, PartedString &P){
	for (;A<A_end;++A){
		auto_part(string {*A},P);
	}
}
int main(int argc,char **argv){
	if (argc<3) return 1;
	cout << "Compiled on " << __DATE__ << " at " << __TIME__ << '\n';
	unsigned C=1;
	char** A = argv+1;
	if (what_flag(argv[1])==4){
		C=strlen(argv[2]);
		if (C==0 || argv[2][0]=='0') return 1;
		for (unsigned i=0;i<C;++i){
			if (!isdigit(argv[2][i])) return 1;
		} // return 1 means -c was caught but following argument was invalid
		C=atoi(argv[2]);
		A = argv + 3;
	}
	PartedString M;
	literal_storer(A,argv+argc,M);
	for (((A==argv+argc) ? dry_run(M) : part_storer(A,argv+argc,M));C>0;--C){
		cout << M << '\n';
	}
	return 0;
}
