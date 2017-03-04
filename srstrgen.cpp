#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <regex>
#include <random>

using namespace std;

mt19937 RNG(time(0));

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

string b_incs(const char* I){
	/*
	const char *lo= "abcdefghijklmnopqrstuvwxyz",
		*up= "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		*dt= "0123456789",
		*s1= "!#$%&()*+-=@^_`~",
		*s2= "\"',./:;<>?[\\]{|}",
		*sp= " ";
	const char *C[]= {lo,up,dt,s1,s2,sp};
	*/
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
struct ST {
	unsigned L = 0, W = 0;
	string * I;
	string * D;
	ST(string &s){
		I=&s;
		L=0;
		W=1;
	}
	ST(string &s,unsigned c){
		I=&s;
		L=c;
		W=1;
	}
	ST(string &s,string &d,unsigned r){
		I=&s;
		D=&d;
		W=r;
		L=0;
	}
	ST(string &s,unsigned c,string &d,unsigned r){
		I=&s;
		D=&d;
		L=c;
		W=r;
	}
};
ostream& operator<<(ostream& out,ST &P){
	if (P.L!=0){
		uniform_int_distribution<unsigned> r(0,(*P.I).length()-1);
		for (unsigned w=P.W;w>0;--w){
			for (unsigned l=0;l<P.L;++l) out<<(*P.I)[r(RNG)];
			if (w!=1) out<<*P.D;
			//cout<<w<<'\n';
		}
	} else {
		for (unsigned w=P.W;w>0;--w){
			out<<*P.I;
			if (w!=1) out<<*P.D;
			//cout<<w<<'\n';
		}
	}
	return out;
}

int literal_storer(int C,char **A,vector<string> &lits){
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
vector<ST> part_storer(int I,int C,char **A,vector<string> &lits){
	unsigned U[4];
	unsigned c;
	string t;
	vector<ST> parts;
	regex r("[0-9]+");
	for (int i=I;i<C;++i){
		t=string {A[i]};
		c=0;
		for(auto j=sregex_iterator(t.begin(),t.end(),r);j!=sregex_iterator() && c<4;++j){
			U[c]=atoi((*j).str().c_str());
			//cout << U[c] << '\n';
			c++;
		}
		//cout << "counted " << c << '\n';
		/*
		for(unsigned i=0;i<c;++i){ cout << U[i] << '\n'; }
		*/

		switch (c){
			/*
			case 0:
				lits.push_back(t);
				parts.push_back(ST(lits.back()));
				break;
			*/
			case 1:
				if (U[0]<lits.size()){
					//cout << "part with lit index " << U[0] << '\n';
					parts.push_back(ST(lits.at(U[0])));
				}
				break;
			case 2:
				if (U[0]<lits.size()){
					//cout << "part with lit index " << U[0] << '\n';
					//cout << "length of " << U[1] << '\n';
					parts.push_back(ST(lits.at(U[0]),U[1]));
				}
				break;
			case 3:
				if (U[0]<lits.size() && U[1]<lits.size()){
					parts.push_back(ST(lits.at(U[0]),lits.at(U[1]),U[2]));
					//cout << "part with lit index " << U[0] << '\n';
					//cout << "length of " << U[1] << '\n';
					//cout << "part with dim index " << U[2] << '\n';
				}
				break;
			case 4:
				if (U[0]<lits.size() && U[2]<lits.size()){
					parts.push_back(ST(lits.at(U[0]),U[1],lits.at(U[2]),U[3]));
					//cout << "part with lit index " << U[0] << '\n';
					//cout << "length of " << U[1] << '\n';
					//cout << "part with dim index " << U[2] << '\n';
					//cout << "wordcount of " << U[3] << '\n';
				}
				break;
			default:
					cout << t <<" is an invalid option.";
				break;
		}
	}

	return parts;
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
	//cout << "MY LITERALS" << '\n';
	//for (auto l : lits) cout << '\t' << l << '\n';
	//C = atoi(argv[1]);
	//cout << C << '\n';
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
