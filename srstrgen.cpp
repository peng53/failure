#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

vector<char> remove_excs(vector<char> I, char *E){
	sort(I.begin(),I.end());
	vector<char> O;
	O.reserve(I.size());
	auto i = I.begin();
	auto e_len = strlen(E);
	sort(E,E+e_len);
	for (char *e=E;e!=E+e_len;++e){
		if (*i<=*e){
			for (;i!=I.end();O.push_back(*i++)){
				if (*i==*e){ ++i; break; }
			}
			//while(i!=I.end()){
				//if (*i==*e){ ++i; break; }
				//O.push_back(*i++);
			//}
		}
	}
	O.insert(O.end(),i,I.end());
	return O;
}

vector<char> build_incs(char* I){
	const char lo[]= "abcdefghijklmnopqrstuvwxyz",
		up[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		dt[]= "0123456789",
		s1[]= "!#$%&()*+-=@^_`~",
		s2[]= "\"',./:;<>?[\\]{|}",
		sp[]= " ";
	const char *C[]= {lo,up,dt,s1,s2,sp};
	unsigned C_L[] = {26,26,10,16,16,1};
	bool inc[6] = {};
	unsigned c_len = 0;
	for (unsigned i=0;i<6;++i){
		inc[i] = I[i]=='1';
		if (inc[i]) c_len+=C_L[i];
	}
	vector <char> CV;
	CV.reserve(c_len);
	for (unsigned i=0;i<6;++i){
		if (inc[i]) CV.insert(CV.end(),C[i],C[i]+C_L[i]);
	}
	return CV;
}

template<typename T, typename Ts>
void rand_word(vector<T> &C, unsigned L,unsigned W,ostream &sout,Ts S){
	mt19937 rng(time(0));
	uniform_int_distribution<unsigned> r(0,C.size()-1);
	for (auto w=W;w>0;--w){
		for (auto l=L;l>0;--l) sout << C[r(rng)];
		if (w!=1) sout << S;
	}
	sout << '\n';
}

int main(int argc, char *argv[]){
	if (argc<6){
		cout << "Missing arguments..\n";
		return 1;
	}
	if (strlen(argv[1])<6){
		cout << "Need to specify all includes and excludes\n";
		return 1;
	}
	vector<char> C = remove_excs(build_incs(argv[1]),argv[2]);
	//for (char c : C) cout<<c;
	unsigned L = atoi(argv[3]), W = atoi(argv[4]);
	rand_word(C,L,W,cout,argv[5]);
	return 0;
}
