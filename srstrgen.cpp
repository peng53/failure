#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <regex>
#include <random>

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
	string * I;
	string * D;
	ST(string &s) : I(&s){}
		//I=&s;L=0;W=1;}
	ST(string &s,unsigned c) : L(c),I(&s){}
		//I=&s;L=c;W=1;}
	ST(string &s,string &d,unsigned r) : W(r),I(&s),D(&d){}
		//I=&s;D=&d;W=r;L=0;}
	ST(string &s,unsigned c,string &d,unsigned r) : L(c),W(r),I(&s),D(&d){}
		//I=&s;D=&d;L=c;W=r;}
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
void auto_part(const string &t,vector<string> &lits,vector<ST> &P){
	if (t.length()==0): return;
	unsigned U[4];
	unsigned c = 0;
	size_t lits_s = lits.size();
	regex r("[0-9]+");
	for(auto j=sregex_iterator(t.begin(),t.end(),r);j!=sregex_iterator() && c<4;++j){
		U[c++]=atoi((*j).str().c_str());
	}
	if (c==0){
		lits.push_back(t);
		P.push_back(ST(lits.back()));
	} else if (U[0]<lits_s){
		switch (c){
		case 1: P.push_back(ST(lits.at(U[0]))); break;
		case 2: P.push_back(ST(lits.at(U[0]),U[1])); break;
		case 3: if (U[1]<lits_s) P.push_back(ST(lits.at(U[0]),lits.at(U[1]),U[2])); break;
		case 4: if (U[2]<lits_s) P.push_back(ST(lits.at(U[0]),U[1],lits.at(U[2]),U[3])); break;
		}
	}
}
vector<ST> part_storer(const int I,const int C,char **A,vector<string> &lits){
	vector<ST> parts;
	for (int i=I;i<C;++i) auto_part(string {A[i]},lits,parts);
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
	cout << "Compiled on " << __DATE__ << " at " << __TIME__ << '\n';
	unsigned C=strlen(argv[1]);
	if (C==0 || argv[1][0]=='0') return 1;
	for (unsigned i=1;i<C;++i){
		if (!isdigit(argv[1][i])) return 1;
	}
	vector<string> lits;
	int i = literal_storer(argc,argv,lits);
	vector<ST> parts = part_storer(i,argc,argv,lits);
	for (C=atoi(argv[1]);C>0;--C){
		for (auto p : parts){
			cout << p;
		}
		cout <<'\n';
	}
	return 0;
}
/*
after -s, possible forms, all of them:
1.		LITERAL_INDEX
2.		LITERAL_INDEX,COUNT
3.		LITERAL_INDEX,DELIMITER_INDEX,WORD_COUNT
4.		LITERAL_INDEX,COUNT,DELIMITER_INDEX,WORD_COUNT

1.		prints literal as is, full string
2.		prints COUNT of random characters from the literal
3.		prints literal as is WORD_COUNT times seperated by DELIMITER_INDEX
4.		prints COUNT of random chars from the literal WORD_COUNT times seperated by DELIMITER_INDEX

Total lengths of the options..
1.		length of the literal
2.		count
3.		length of literal*WORD_COUNT + length of delimiter*(WORD_COUNT-1)
4.		count*WORD_COUNT + length of delimiter*(WORD_COUNT-1)
*/
