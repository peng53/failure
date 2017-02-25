#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

mt19937 gen_rng(time(0));

vector<char> remove_excs(vector<char> I, char *E){
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
	unsigned W, L;
	vector<char> C;
	const char *DM;
	Part(const vector<char> &_C,const unsigned _L,const unsigned _W,const char *_DM){
		C=_C;W=_W;L=_L;DM=_DM;
	}
	Part(const char* _C,unsigned _W,const char *_DM,vector<vector<char>> &stored){
		stored.push_back(vector<char> {_C,_C+strlen(_C)});
		C=stored[stored.size()-1];
		W=_W;L=0;DM=_DM;
	}
};
ostream& operator<<(ostream& t_out, Part &p){
	if (p.L==0){
		for (auto w=p.W;w>0;--w){
			for (char c : p.C) t_out << c;
			if (w!=1) t_out << p.DM;
		}
	} else {
		uniform_int_distribution<unsigned> r(0,p.C.size()-1);
		for (auto w=p.W;w>0;--w){
			for (auto l=p.L;l>0;--l) t_out << p.C[r(gen_rng)];
			if (w!=1) t_out << p.DM;
		}
	}
	return t_out;
}
const char* partcstr(Part &p){
	stringstream sout;
	sout << p;
	const string rs = sout.str();
	const char* rs2 = rs.c_str();
	return rs2;
}
vector<char> charvec(const char *cc){
	return vector<char> {cc,cc+strlen(cc)};
}
//const char* out_to_string(stringstream &sout){
	//const string rs = sout.str();
	//const char* rs2 = rs.c_str();
	//return rs2;
//}
/*
template<typename Stringlike, typename StreamType>
void repeat_word_delimited(const Stringlike S, const unsigned W, const Stringlike DM, StreamType &sout){
	for (auto w=W;w>0;--w) sout << S << DM;
}*/

//template<typename T, typename Ts, typename StreamType>
//void rand_word_uni(const T &C, const size_t C_len, const unsigned L, const unsigned W,StreamType &sout,const Ts S){
	//mt19937 rng(time(0));
	//uniform_int_distribution<unsigned> r(0,C_len-1);
	//for (auto w=W;w>0;--w){
		//for (auto l=L;l>0;--l) sout << C[r(rng)];
		//if (w!=1) sout << S;
	//}
//}
//template<typename T, typename Ts>
//const char* stir_mix(const T &C, const size_t C_len, const unsigned L, const unsigned W, const Ts S){
	//stringstream t_out;
	//rand_word_uni(C,C_len,L,W,t_out,S);
	//return out_to_string(t_out);
//}

//extern "C" const char* plen(const char *I, char *E, const unsigned L, const unsigned W, const char *S){
	//vector<char> C = remove_excs(build_incs(I),E);
	//return stir_mix(C,C.size(),L,W,S);
//}

//extern "C" const char* instant_coffee(const char *C, const size_t C_len, const unsigned L, const unsigned W, const char *S){
	//return stir_mix(C,C_len,L,W,S);
//}

const char* stir_mix(const vector<char> &C, const unsigned L, const unsigned W, const char *S){
	Part p = Part(C,L,W,S);
	return partcstr(p);
}

extern "C" const char* plen(const char*I,char *E, const unsigned L, const unsigned W, const char *S){
	vector<char> CV = build_incs(I);
	const vector<char> C = remove_excs(CV,E);
	return stir_mix(C,L,W,S);
	//Part p = Part(C,L,W,S);
	//return partcstr(p);
}
extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S){
	const vector<char> vC = charvec(C);
	return stir_mix(vC,L,W,S);
	//Part p = Part(vC,L,W,S);
	//return partcstr(p);
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
/*void add_stored(vector<vector<char>> &stored, vector<char> C){
	stored.push_back(C);
}*/

int handle_g(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<6 || !good_number(arg[i+3]) || !good_number(arg[i+4])) return ++i;
	unsigned L = atoi(arg[i+3]), W = atoi(arg[i+4]);
	vector<char> CV = build_incs(arg[i+1]);
	stored.push_back(remove_excs(CV,arg[i+2]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+5]));
	return i+5;
}
int handle_u(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5 || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	stored.push_back(charvec(arg[i+1]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+4]));
	return i+4;
}
int handle_q(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5 || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	stored.push_back(build_incs(arg[i+1]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+4]));
	return i+4;
}
int handle_r(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5 || !good_number(arg[i+1]) || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned I = atoi(arg[i+1]), L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	if (I>stored.size()) return ++i;
	parts.push_back(Part(stored[I-1],L,W,arg[i+4]));
	return i+4;
}
int handle_c(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<4 || !good_number(arg[i+2])) return ++i;
	unsigned W = atoi(arg[i+2]);
	parts.push_back(Part(arg[i+1],W,arg[i+3],stored));
	return i+3;
}

int main(int argc, char *argv[]){
	vector<Part> parts;
	vector<vector<char>> stored;
	int (*handles[])(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored)=
			{handle_g,handle_u,handle_q,handle_c,handle_r};
	int i = 1;
	int i_c;
	while (i<argc){
		i_c = flag_catcher(argv[i]);
		if (i_c==-1) ++i;
		else i=handles[i_c](argv,i,argc,parts,stored);
	}
	for (auto P : parts) cout << P;
	cout << '\n';
	return 0;
}
