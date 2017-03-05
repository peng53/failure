#ifdef _DEBUG
#define DBOUT cout // or any other ostream
#else
#define DBOUT 0 && cout
#endif

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <random>
#include <sstream>
#include <list>
#include <stack>

using namespace std;

mt19937 RNG(time(0));

string b_incs(const char* I){
	const char *C[]= {
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"0123456789",
		"!#$%&()*+-=@^_`~",
		"\"',./:;<>?[\\]{|}",
		" "
	};
	const unsigned C_L[] = {26,26,10,16,16,1};
	string CV;
	for (unsigned i=0;i<6;++i){
		if (I[i]=='1'){
			CV.reserve(CV.size()+C_L[i]);
			CV.insert(CV.end(),C[i],C[i]+C_L[i]);
		}
	}
	return CV;
}
string remove_excs(string &I, char *E){
	sort(I.begin(),I.end());
	string O;
	O.reserve(I.length());
	auto i = I.cbegin();
	auto e_len = strlen(E);
	sort(E,E+e_len);
	for (char *e=E;e!=E+e_len;++e){
		if (*i<=*e){
			for (;i!=I.cend();O.push_back(*i++)){
				if (*i==*e){ ++i; break; }
			}
		}
	}
	O.insert(O.end(),i,I.cend());
	return O;
}
int what_flag(const char *s){
	switch (strlen(s)){
	case 0: return 0;
	case 2:
		switch (s[1]){
		case 's': return 1;
		case 'b': return 2;
		case 'B': return 3;
		}
	}
	return 9;
}
int literal_storer(const int C,char **A,vector<string> &lits){
	string I;
	for (int i=2;i<C;++i){
		switch(what_flag(A[i])){
		case 0: break;
		case 1: return ++i;
		case 2:
			if (i+2<C && strlen(A[i+1])==6){
				I = b_incs(A[++i]);
				lits.push_back(remove_excs(I,A[++i]));
			} else lits.push_back(string {A[i]});
			break;
		case 3:
			if (i+1<C && strlen(A[i+1])==6) lits.push_back(b_incs(A[++i]));
			else lits.push_back(string {A[i]});
			break;
		default: lits.push_back(string {A[i]}); break;
		}
	}
	return C;
}
struct ST {
	unsigned L = 0, W = 1;
	string * I = nullptr;
	string * D = nullptr;
	ST(string &s) : I(&s){}
	ST(string &s,unsigned c) : L(c),I(&s){}
	ST(string &s,string &d,unsigned r) : W(r),I(&s),D(&d){}
	ST(string &s,unsigned c,string &d,unsigned r) : L(c),W(r),I(&s),D(&d){}
};
ostream& operator<<(ostream& out,ST &P){
	if (P.L!=0){
		uniform_int_distribution<unsigned> r(0,(*P.I).length()-1);
		for (unsigned w=P.W;w>0;--w){
			for (unsigned l=0;l<P.L;++l) out<<(*P.I)[r(RNG)];
			if (w!=1) out<<*P.D;
		}
	} else {
		for (unsigned w=P.W;w>0;--w){
			out<<*P.I;
			if (w!=1) out<<*P.D;
		}
	}
	return out;
}
unsigned trialnerror(unsigned U[4],const string &t){
	unsigned c=0;
	unsigned b, n;
	size_t l=t.length();
	for (unsigned i=0;i<l && c<4;++c){
		n = 1;
		while (i<l && !isdigit(t[i])) ++i; // find start
		if (i==l) return 0; // no digits..
		for (b=i++;(i<l && isdigit(t[i++]));++n) continue; //count digits
		U[c] = atoi(t.substr(b,n).c_str());
	}
	return c;
}
void auto_part(const string &t,vector<string> &lits,list<ST> &P,stack<string*> &emerg){
	if (t.length()==0) return;
	unsigned U[4];
	unsigned lits_s = lits.size(), c = trialnerror(U,t);
	if (c==0){
		emerg.push(new string{t});
		DBOUT << emerg.top() << " made\n";
		P.push_back(ST(*emerg.top()));
	} else if (U[0]<lits_s){
		switch (c){
		case 1: P.push_back(ST(lits.at(U[0]))); break;
		case 2: P.push_back(ST(lits.at(U[0]),U[1])); break;
		case 3: if (U[1]<lits_s) P.push_back(ST(lits.at(U[0]),lits.at(U[1]),U[2])); break;
		case 4: if (U[2]<lits_s) P.push_back(ST(lits.at(U[0]),U[1],lits.at(U[2]),U[3])); break;
		}
	}
}
list<ST> part_storer(int I,const int C,char **A,vector<string> &lits,stack<string*> &emerg){
	list<ST> parts;
	for (int i=I;i<C;++i) auto_part(string {A[i]},lits,parts,emerg);
	return parts;
}
const char* stir_mix(string &C, const unsigned L, const unsigned W, const char *S){
	string sS=S;
	ST p(C,L,sS,W);
	stringstream sout;
	sout << p;
	const string rs = sout.str();
	const char* rs2 = rs.c_str();
	return rs2;
}
extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S){
	string sC=C;
	return stir_mix(sC,L,W,S);
}
extern "C" const char* plen(const char *I,char *E, const unsigned L, const unsigned W, const char *S){
	string CV=b_incs(I);
	string C=remove_excs(CV,E);
	return stir_mix(C,L,W,S);
}
int main(int argc,char **argv){
	if (argc==1) return 1;
	DBOUT << "Compiled on " << __DATE__ << " at " << __TIME__ << '\n';
	unsigned C=strlen(argv[1]);
	if (C==0 || argv[1][0]=='0') return 1;
	for (unsigned i=0;i<C;++i){
		if (!isdigit(argv[1][i])) return 1;
	}
	vector<string> lits;
	int i = literal_storer(argc,argv,lits);
	stack<string*> emerg;
	list<ST> parts = part_storer(i,argc,argv,lits,emerg);
	for (C=atoi(argv[1]);C>0;--C){
		for (auto p : parts) cout << p;
		cout <<'\n';
	}
	for(;!emerg.empty();emerg.pop()){ DBOUT<< emerg.top() << " deleted\n"; delete emerg.top();}
	return 0;
}
