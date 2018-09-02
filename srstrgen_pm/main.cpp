#include <string>
#include <cstring>
#include <iostream>
#include "b_inc.h"
#include "ste.h"

using std::cout;
using std::string;

static int what_flag(const char *s){
	switch (strlen(s)){
	case 0: return 0;
	case 2:
		switch (s[1]){
		case 's': return 1;
		case 'b': return 2;
		case 'B': return 3;
		case 'c': return 4;
		case 'a': return 5;
		}
	}
	return 9;
}
static void literal_storer(char** &A,char **A_end,PartedString &P){
	for (BInc s;A!=A_end;++A){
		switch(what_flag(*A)){
		case 0: break;
		case 1: ++A; return;
		case 2:
			if (A+2<A_end && strlen(*(A+1))==6){
				s = BInc(*(++A));
				s.rem(*(++A));
				P << s.rep();
				break;
			}
		case 3:
			if (A+1<A_end && strlen(*(A+1))==6){
				s = BInc(*(++A));
				P << s.rep();
				break;
			}
		case 5:
			if (A+2<A_end && strlen(*(A+1))==6){
				s = BInc(*(++A));
				s.add(*(++A));
				P << s.rep();
				break;
			}
		default: P << (*A);
		}
	}
}
static unsigned justnumbers(unsigned U[],const unsigned m,const string &t){
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
static void dry_run(PartedString &P){
	const unsigned lits = P.lits_size();
	P << "\n";
	for (unsigned i=0;i<lits;++i){
		P << i << lits;
	}
}
static void auto_part(const string &t, PartedString &P){
	if (t.length()==0) return;
	unsigned U[4];
	const unsigned lits_s = P.lits_size(), c = justnumbers(U,4,t);
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
static void part_storer(char** &A, char** A_end, PartedString &P){
	for (;A<A_end;++A){
		auto_part((*A),P);
		// using implicit char* to const string&
	}
}
int main(int argc,char **argv){
	/*
	To call:
		First arg can be -c N
		if one wants multiple strings,
		if omited, only 1 string is printed.
		N must be a number.

		Next any number of 'parts'
		'-B' takes a 6 char boolstring of what chars to include
		'-b' is same as above but has second arg which is removed from that set
		'-a' is same as -b but adds second arg to that set
		some string that doesn't start with -
		'-c' seem to have been an option to use a coin flip, but seems to be
			disabled right now?

		After the parts, one does the output string, which is signals by
		the '-s' flag.
		Arguments following '-s' are either:
			1 to 4 numbers seperated by non-space
			some string, note: - is available now.

		More on the output string:
			No parts that are prepended by '-' can be generated here.
			'-' is now intepreted as a string.

			Depending on how many numbers are in an argument, different things
			are outputed in a 'segment', (that being an argument in an argument)
			All indices are 0 based.

			# : effect
			1 : outputs part at index
				Note: parts are really just strings
			2	first number acts as part index
				second acts as # of chars to output (random)
			3 : first number acts as part index
				second acts as a 'delimiter part'
				third acts as # of times to print first part.
				the first part sandwiches the second part.
			4 : acts as if you used 2 args, repeats it as many
				times as the 4th arg sandwiching the 3rd arg
				as part index (delimiter)

			If 'some string' in the '-s' section, it will appear output string
			as a segment (like 1 arg) but will also be available as a part.
			however, if it is referenced before is it introduced, it will not appear
			in the output.

			[0th part] -s 1 [1st part (string only)]
			will print nothing as 1 didn't exist when it was referenced, but:
			[0th part] -s [1st part (string only)] 1
			will print the [1st part] twice (without delimiter).

			Also note, one can omit the '-s'. Doing so calls a 'dry run'.
			This prints out all parts seperated by linefeed.

		So to call,
		./binary [-c N] [[part0] [part1]] [-s [segment] [segment]]
		No arguments are actually required.
	*/
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
