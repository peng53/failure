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

template<typename Container>
vector<char> remove_excs(Container I, char *E){
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

template<typename Indexable>
vector<char> build_incs(const Indexable I){
	const char lo[]= "abcdefghijklmnopqrstuvwxyz",
		up[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		dt[]= "0123456789",
		s1[]= "!#$%&()*+-=@^_`~",
		s2[]= "\"',./:;<>?[\\]{|}",
		sp[]= " ";
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

const char* out_to_string(stringstream &sout){
	const string rs = sout.str();
	const char* rs2 = rs.c_str();
	return rs2;
}

template<typename Stringlike, typename StreamType>
void repeat_word_delimited(const Stringlike S, const unsigned W, const Stringlike DM, StreamType &sout){
	for (auto w=W;w>0;--w) sout << S << DM;
}

template<typename T, typename Ts, typename StreamType>
void rand_word_uni(const T &C, const size_t C_len, const unsigned L, const unsigned W,StreamType &sout,const Ts S){
	mt19937 rng(time(0));
	uniform_int_distribution<unsigned> r(0,C_len-1);
	for (auto w=W;w>0;--w){
		for (auto l=L;l>0;--l) sout << C[r(rng)];
		if (w!=1) sout << S;
	}
}
template<typename T, typename Ts>
const char* stir_mix(const T &C, const size_t C_len, const unsigned L, const unsigned W, const Ts S){
	stringstream t_out;
	rand_word_uni(C,C_len,L,W,t_out,S);
	return out_to_string(t_out);
}

extern "C" const char* plen(const char *I, char *E, const unsigned L, const unsigned W, const char *S){
	vector<char> C = remove_excs(build_incs(I),E);
	return stir_mix(C,C.size(),L,W,S);
}

extern "C" const char* instant_coffee(const char *C, const size_t C_len, const unsigned L, const unsigned W, const char *S){
	return stir_mix(C,C_len,L,W,S);
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
	/*
	if (!strcmp(f,"-g")) return 0;
	else if (!strcmp(f,"-u")) return 1;
	else if (!strcmp(f,"-q")) return 2;
	else if (!strcmp(f,"-c")) return 3;
	*/
	return -1;
}

template<typename StreamType>
void total_string_age(char *cs[], size_t cs_len, StreamType &sout){
	for (unsigned i=0;i<cs_len-1;++i){
		switch (flag_catcher(cs[i])){
			case 0:
				cout << "caught a -g" << '\n';
				break;
			case 1:
				cout << "caught a -u" << '\n';
				break;
			case 2:
				cout << "caught a -q" << '\n';
				break;
			case 3:
				cout << "caught a -c" << '\n';
				break;
			default:
				cout << cs[i] << '\n';
				break;
		}
	}
}

mt19937 gen_rng(time(0));

struct Part {
	unsigned W, L;
	vector<char> C;
	const char *DM;
	Part(vector<char> &_C,unsigned _L,unsigned _W,const char *_DM){
		C=_C;W=_W;L=_L;DM=_DM;
	}
	Part(const char* _C,unsigned _W,const char *_DM,vector<vector<char>> &stored){
		stored.push_back(vector<char> {_C,_C+strlen(_C)});
		C=stored[stored.size()-1];
		W=_W;
		L=0;
		DM=_DM;
	}
};
bool good_number(const char* D){
	size_t l = strlen(D);
	if (l==0 || D[0]=='0') return 0;
	for (unsigned i=1;i<l;++i){
		if (!isdigit(D[i])) return 0;
	}
	return 1;
}
void add_stored(vector<vector<char>> &stored, vector<char> C){
	stored.push_back(C);
}
vector<char> charvec(const char *cc){
	return vector<char> {cc,cc+strlen(cc)};
}
int handle_g(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<6) return ++i;
	if (!good_number(arg[i+3]) || !good_number(arg[i+4])) return ++i;
	unsigned L = atoi(arg[i+3]), W = atoi(arg[i+4]);
	vector<char> CV = build_incs(arg[i+1]);
	stored.push_back(remove_excs(CV,arg[i+2]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+5]));
	return i+5;
}
int handle_u(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5) return ++i;
	if (!good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	stored.push_back(charvec(arg[i+1]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+4]));
	return i+4;
}
int handle_q(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5) return ++i;
	if (!good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	stored.push_back(build_incs(arg[i+1]));
	parts.push_back(Part(stored[stored.size()-1],L,W,arg[i+4]));
	return i+4;
}
int handle_c(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<4) return ++i;
	if (!good_number(arg[i+2])) return ++i;
	unsigned W = atoi(arg[i+2]);
	parts.push_back(Part(arg[i+1],W,arg[i+3],stored));
	return i+3;
}
int handle_r(char* arg[],unsigned i, int arc,vector<Part> &parts,vector<vector<char>> &stored){
	if (arc-i<5) return ++i;
	if (!good_number(arg[i+1]) || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	unsigned I = atoi(arg[i+1]), L = atoi(arg[i+2]), W = atoi(arg[i+3]);
	if (I>stored.size()) return ++i;
	parts.push_back(Part(stored[I-1],L,W,arg[i+4]));
	return i+4;
}
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

/*
struct SPart {
	unsigned W;
	const char * DM;
};
template <class C_type> struct GPart: SPart {
	unsigned L;
	size_t C_len;
	C_type C;
	template<typename Ct> GPart(Ct &_C,size_t _C_len, unsigned _L, unsigned _W, const char* _DM){
		C = _C;
		C_len = _C_len;
		L = _L;
		W = _W;
		DM = _DM;
	}
};
template <class C_type> ostream& operator<<(ostream& t_out, GPart<C_type> & p){
	uniform_int_distribution<unsigned> r(0,p.C_len-1);
	for (auto w=p.W;w>0;--w){
		for (auto l=p.L;l>0;--l) t_out << p.C[r(gen_rng)];
		if (w!=1) t_out << p.DM;
	}
	return t_out;
}
struct CPart: SPart {
	const char *C;
	CPart(const char *_C, unsigned _W, const char* _DM){
		C = _C;
		W = _W;
		DM = _DM;
	}
};
ostream& operator<<(ostream& t_out, CPart &p){
	for (auto w=p.W;w>0;--w){
		t_out << p.C;
		if (w!=1) t_out << p.DM;
	}
	return t_out;
}
*/


int main(int argc, char *argv[]){
	/*
	if (argc<6){
		cout << "Missing arguments..\n";
		return 1;
	}
	if (strlen(argv[1])<6){
		cout << "Need to specify all includes and excludes\n";
		return 1;
	}
	const unsigned L = atoi(argv[3]), W = atoi(argv[4]);
	vector<char> C = remove_excs(build_incs(argv[1]),argv[2]);
	rand_word_uni(C,C.size(),L,W,cout,argv[5]);
	cout << '\n';
	*/
	stringstream t_out;
	//total_string_age(argv,(size_t)argc,t_out);
	/*
	vector<GPart<vector<char>>> myparts;
	vector<char> C = {'1','2','3','4'};
	const char *x = "-";
	unsigned L = 3;
	unsigned W = 3;
	GPart<vector<char>> t = GPart<vector<char>>(C,C.size(),L,W,x);

	myparts.push_back(t);

	string C3 = "1234";
	//const char *x = "-";
	GPart<string> t3 = GPart<string>(C3,C3.length(),L,W,x);
	cout << t3 << '\n';

	const char *C2 = "5678a";
	const char *x2 = "/";
	CPart t2 = CPart(C2,W,x2);
	cout << t2 << '\n';
	cout << myparts[0] << '\n';
	*/
	vector<Part> parts;
	vector<vector<char>> stored;
	int i = 1;
	while (i<argc){
		switch(flag_catcher(argv[i])){
			case 0:i=handle_g(argv,i,argc,parts,stored);break;
			case 1:i=handle_u(argv,i,argc,parts,stored);break;
			case 2:i=handle_q(argv,i,argc,parts,stored);break;
			case 3:i=handle_c(argv,i,argc,parts,stored);break;
			case 4:i=handle_r(argv,i,argc,parts,stored);break;
			default:++i;break;
		}
	}
	/*
	//vector<char> c1 {'1','2','3'};
	//vector<char> c2 {'a','b','c'};
	const char* happy = "happy";
	//vector<char> c3 {happy,happy+strlen(happy)};
	//vector<char> c3 = charvec(happy);
	const char* dm = "-";
	const char* dm2 = "/";
	vector<vector<char>> stored;
	add_stored(stored,{'1','2','3'});
	add_stored(stored,{'a','b','c'});
	add_stored(stored,charvec(happy));
	parts.push_back(Part(stored[0],3,2,dm));
	parts.push_back(Part(stored[1],3,2,dm2));
	parts.push_back(Part(stored[2],0,2,dm2));
	parts.push_back(Part(stored[0],4,4,dm2));
	parts.push_back(Part(happy,3,dm,stored));
	//cout << parts[0] << parts[1] << parts[2] << '\n';
	*/
	for (auto P : parts){
		cout << P;
	}
	cout << '\n';
	return 0;
}
