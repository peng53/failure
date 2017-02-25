#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

mt19937 gen_rng(time(0));

vector<char> remove_excs(vector<char> &I, char *E){
	sort(I.begin(),I.end());
	vector<char> O;
	O.reserve(I.size());
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
vector<char> build_incs(const char *I){
	const char *lo= "abcdefghijklmnopqrstuvwxyz",
		*up= "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		*dt= "0123456789",
		*s1= "!#$%&()*+-=@^_`~",
		*s2= "\"',./:;<>?[\\]{|}",
		*sp= " ";
	const char *C[]= {lo,up,dt,s1,s2,sp};
	const unsigned C_L[] = {26,26,10,16,16,1};
	vector <char> CV;
	for (unsigned i=0;i<6;++i){
		if (I[i]=='1'){
			CV.reserve(CV.size()+C_L[i]);
			CV.insert(CV.end(),C[i],C[i]+C_L[i]);
		}
	}
	return CV;
}
struct Part {
	unsigned W, L, I;
	const char *DM;
	Part(unsigned _I,const unsigned _L,const unsigned _W,const char *_DM){
		I=_I;W=_W;L=_L;DM=_DM;
	}
	void out_part(const vector<char> &C, ostream &sout){
		if (L==0){
			for (auto w=W;w>0;--w){
				for (char c : C) sout << c;
				if (w!=1) sout << DM;
			}
		} else {
			uniform_int_distribution<unsigned> r(0,C.size()-1);
			for (auto w=W;w>0;--w){
				for (auto l=L;l>0;--l) sout << C[r(gen_rng)];
				if (w!=1) sout << DM;
			}
		}
	}
};
const char* stir_mix(const vector<char> &C, const unsigned L, const unsigned W, const char *S){
	Part p = Part(0,L,W,S);
	stringstream sout;
	p.out_part(C,sout);
	const string rs = sout.str();
	const char* rs2 = rs.c_str();
	return rs2;
}
extern "C" const char* plen(const char*I,char *E, const unsigned L, const unsigned W, const char *S){
	vector<char> CV = build_incs(I);
	return stir_mix(remove_excs(CV,E),L,W,S);
}
extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S){
	return stir_mix(vector<char> {C,C+strlen(C)},L,W,S);
}
struct Parts {
	vector<vector<char>> stored;
	vector<Part> parts;
	unsigned add_stor(vector<char> &I){
		stored.push_back(I);
		return stored.size()-1;
	}
	unsigned remove_excs_i(vector<char> &I, char *E){
		stored.push_back(remove_excs(I,E));
		return stored.size()-1;
	}
	unsigned charvec(const char *cc){
		stored.push_back(vector<char> {cc,cc+strlen(cc)});
		return stored.size()-1;
	}
	/*
	unsigned remove_excs(vector<char> &I, char *E){
		sort(I.begin(),I.end());
		unsigned r = stored.size();
		stored.push_back(vector<char> {});
		stored[r].reserve(I.size());
		auto i = I.cbegin();
		auto e_len = strlen(E);
		sort(E,E+e_len);
		for (char *e=E;e!=E+e_len;++e){
			if (*i<=*e){
				for (;i!=I.cend();stored[r].push_back(*i++)){
					if (*i==*e){ ++i; break; }
				}
			}
		}
		stored[r].insert(stored[r].end(),i,I.cend());
		return r;
	}
	*/
	void add_part(unsigned I,unsigned L,unsigned W,char* DM){
		parts.push_back(Part(I,L,W,DM));
	}
};
ostream& operator<<(ostream& t_out, Parts &P){
	for (auto p : P.parts) p.out_part(P.stored[p.I],t_out);
	return t_out;
}
int flag_catcher(char *f){
	size_t l = strlen(f);
	if (l!=2 || f[0]!='-') return -1;
	switch (f[1]){
		case 'g': return 0;
		case 'u': return 1;
		case 'q': return 2;
		case 'c': return 3;
		case 'r': return 4;
	}
	return -1;
}
bool good_number(const char* D){
	size_t l = strlen(D);
	if (l==0 || D[0]=='0') return 0;
	for (unsigned i=1;i<l;++i){
		if (!isdigit(D[i])) return 0;
	}
	return 1;
}

int handle_g(char* arg[],unsigned i,int arc,Parts &P){
	if (arc-i<6 || !good_number(arg[i+3]) || !good_number(arg[i+4])) return ++i;
	vector<char> CV = build_incs(arg[i+1]);
	P.add_part(P.remove_excs_i(CV,arg[i+2]),atoi(arg[i+3]),atoi(arg[i+4]),arg[i+5]);
	return i+5;
}
int handle_u(char* arg[],unsigned i,int arc,Parts &P){
	if (arc-i<5 || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	P.add_part(P.charvec(arg[i+1]),atoi(arg[i+2]),atoi(arg[i+3]),arg[i+4]);
	return i+4;
}
int handle_q(char* arg[],unsigned i, int arc,Parts &P){
	if (arc-i<5 || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	vector<char> C = build_incs(arg[i+1]);
	P.add_part(P.add_stor(C),atoi(arg[i+2]),atoi(arg[i+3]),arg[i+4]);
	return i+4;
}
int handle_r(char* arg[],unsigned i, int arc,Parts &P){
	if (arc-i<5 || !good_number(arg[i+1]) || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned I = atoi(arg[i+1]);
	if (I>P.stored.size()) return ++i;
	P.add_part(I-1,atoi(arg[i+2]),atoi(arg[i+3]),arg[i+4]);
	return i+4;
}
int handle_c(char* arg[],unsigned i, int arc,Parts &P){
	if (arc-i<4 || !good_number(arg[i+2])) return ++i;
	P.add_part(P.charvec(arg[i+1]),0,atoi(arg[i+2]),arg[i+3]);
	return i+3;
}

int main(int argc, char *argv[]){
	Parts parts;
	int (*handles[])(char* arg[],unsigned i, int arc,Parts &parts)=
			{handle_g,handle_u,handle_q,handle_c,handle_r};
	int i = 1;
	int i_c;
	while (i<argc){
		i_c = flag_catcher(argv[i]);
		if (i_c==-1) ++i;
		else i=handles[i_c](argv,i,argc,parts);
	}
	cout << parts << '\n';
	return 0;
}
