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
	unsigned e_len = strlen(E);
	sort(E,E+e_len);
	//for (unsigned e_i=0;e_i<e_len;++e_i){
		//while(i!=I.end()){
			//if (*i>E[e_i]) break;
			//if (*i==E[e_i]){
				//++i;
				//break;
			//}
			//O.push_back(*i++);
		//}
	//}
	for (char *e=E;e!=E+e_len;++e){
		if (*i<=*e){
			while(i!=I.end()){
				if (*i==*e){ ++i; break; }
				O.push_back(*i++);
			}
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

int main(int argc, char *argv[]){
	if (argc<3){
		cout << "Missing arguments..\n";
		return 1;
	}
	if (strlen(argv[1])<6){
		cout << "Need to specify all includes and excludes\n";
		return 1;
	}
	vector<char> C = remove_excs(build_incs(argv[1]),argv[2]);
	for (char c : C) cout<<c;
	cout << '\n';
	return 0;
}
