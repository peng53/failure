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
	Part(const unsigned _I,const unsigned _L,const unsigned _W,const char *_DM){
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
	Part p(0,L,W,S);
	stringstream sout;
	p.out_part(C,sout);
	const string rs = sout.str();
	const char* rs2 = rs.c_str();
	return rs2;
}
extern "C" const char* plen(const char *I,char *E, const unsigned L, const unsigned W, const char *S){
	vector<char> CV = build_incs(I);
	return stir_mix(remove_excs(CV,E),L,W,S);
}
extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S){
	return stir_mix(vector<char> {C,C+strlen(C)},L,W,S);
}

class Parts {
	private:
		vector<vector<char>> stored;
		vector<Part> parts;
		friend ostream& operator<<(ostream& t_out,const Parts &P);
	public:
		unsigned add_stor(const vector<char> &I){
			stored.push_back(I);
			return stored.size()-1;
		}
		unsigned add_stor(vector<char> &I, char *E){
			stored.push_back(remove_excs(I,E));
			return stored.size()-1;
		}
		unsigned add_stor(const char *cc){
			stored.push_back(vector<char> {cc,cc+strlen(cc)});
			return stored.size()-1;
		}
		void add_part(unsigned I,unsigned L,unsigned W,char* DM){
			parts.push_back(Part(I,L,W,DM));
		}
		bool has_stor(unsigned I){
			return I<=stored.size();
		}
};
ostream& operator<<(ostream& t_out,const Parts &P){
	for (auto p : P.parts) p.out_part(P.stored[p.I],t_out);
	return t_out;
}
int flag_catcher(const char *f){
	if (strlen(f)!=2 || f[0]!='-') return -3;
	switch (f[1]){
		case 'b': return 1;
		case 'B': return 2;
		case 's': return 3;
		case 'g': return 0;
		case 'u': return 1;
		case 'q': return 2;
		case 'c': return 3;
		case 'r': return 4;
	}
	return -3;
}
bool good_number(const char* D){
	size_t l = strlen(D);
	if (l==0 || D[0]=='0') return 0;
	for (unsigned i=1;i<l;++i){
		if (!isdigit(D[i])) return 0;
	}
	return 1;
}
/*
int handle_g(char* arg[],unsigned i,int arc,Parts &P){
	if (arc-i<6 || !good_number(arg[i+3]) || !good_number(arg[i+4])) return ++i;
	vector<char> CV = build_incs(arg[i+1]);
	P.add_part(P.add_stor(CV,arg[i+2]),atoi(arg[i+3]),atoi(arg[i+4]),arg[i+5]);
	return i+5;
}
int handle_u(char* arg[],unsigned i,int arc,Parts &P){
	if (arc-i<5 || !good_number(arg[i+2]) || !good_number(arg[i+3])) return ++i;
	P.add_part(P.add_stor(arg[i+1]),atoi(arg[i+2]),atoi(arg[i+3]),arg[i+4]);
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
	if (!P.has_stor(I)) return ++i;
	P.add_part(I-1,atoi(arg[i+2]),atoi(arg[i+3]),arg[i+4]);
	return i+4;
}
int handle_c(char* arg[],unsigned i, int arc,Parts &P){
	if (arc-i<4 || !good_number(arg[i+2])) return ++i;
	P.add_part(P.add_stor(arg[i+1]),0,atoi(arg[i+2]),arg[i+3]);
	return i+3;
}
*/


struct SmallPart {
	unsigned L, W;
	string* I;
	string* Iw;

	SmallPart(string* _I, unsigned _L, string* _Iw, unsigned _W){
		I=_I;
		Iw=_Iw;
		L=_L;
		W=_W;
	}
//	*/
	SmallPart(string* _I){
		I=_I;
		Iw=nullptr;
		L=0;
		W=0;
	}

};

string remove_excs_new(string &I, char *E){
	auto e_len = strlen(E);
	sort(I.begin(),I.end());
	auto i = I.cbegin();
	sort(E,E+e_len);
	string O;
	O.reserve(I.size());
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
string build_incs_b(const char *I){
	const char *lo= "abcdefghijklmnopqrstuvwxyz",
		*up= "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		*dt= "0123456789",
		*s1= "!#$%&()*+-=@^_`~",
		*s2= "\"',./:;<>?[\\]{|}",
		*sp= " ";
	const char *C[]= {lo,up,dt,s1,s2,sp};
	const unsigned C_L[] = {26,26,10,16,16,1};
	string CV;
	for (unsigned i=0;i<6;++i){
		if (I[i]=='1'){
			CV.reserve(CV.length()+C_L[i]);
			CV.insert(CV.end(),C[i],C[i]+C_L[i]);
		}
	}
	return CV;
}
string my_incs(char *I,char *E){
	if (strlen(I)!=6) return string {I};
	string t = build_incs_b(I);
	return remove_excs_new(t,E);
}
void part_maker(const char* s, vector<string> &lits,vector<SmallPart> &parts){
	unsigned l = strlen(s);
	if (l==0) return;
	if (s[0]!='['){
		lits.push_back(string {s});
		string* s = &(lits[lits.size()-1]);
		parts.push_back(SmallPart(s));
	}

}


int main(int argc, char *argv[]){
	/*
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
	*/
	//
	//
	if (argc==1) return 1;
	vector<string> strlit {};
	vector<SmallPart> parts;
	//string t;
	int c_arg = 1;
	cout << c_arg;
	int c_flag = flag_catcher(argv[1]);
	cout << c_flag;
	for (;c_arg<argc && c_flag!=3;c_flag=flag_catcher(argv[++c_arg])){
		cout << "gg";
		switch(c_flag){
			case 3:
				cout << 'h';
				break;
			case 1:
				if (c_arg+2<argc){
					strlit.push_back(my_incs(argv[c_arg+1],argv[c_arg+2]));
					c_arg+=2;
				}
				break;
			case 2:
				if (++c_arg<argc) strlit.push_back(build_incs_b(argv[c_arg]));
				break;
			default:
				strlit.push_back(string {argv[c_arg]});
				break;
		}
	}
	++c_arg;
	for (;c_arg<argc;++c_arg){
		part_maker(argv[c_arg],strlit,parts);
	}
	/*
	for (auto s : strlit){
		cout << s << '\n';
	}*/
	for (auto p : parts){
		cout << *p.I;
		if (p.Iw!=nullptr) cout << *p.Iw;
		cout << '\n';
	}

	return 0;
}
