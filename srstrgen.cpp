#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

vector<char> dif_a_b(vector<char> &I, char *E){
	vector<char> O;
	O.reserve(I.size());
	//vector<char>::reverse_iterator i = I.rbegin();
	//int e_len = strlen(E);
	//sort(E,E+e_len);
	//for (int e_i=e_len-1;e_i>-1;--e_i){
		//while(i!=I.rend()){
			//if (*i<E[e_i]) break;
			//if (*i==E[e_i]){
				//++i;
				//break;
			//} else O.push_back(*i++);//++i;}
		//}
	//}
	//while (i!=I.rend()) O.push_back(*i++);//++i;
	vector<char>::iterator i = I.begin();
	int e_len = strlen(E);
	sort(E,E+e_len);
	for (int e_i=0;e_i<e_len;++e_i){
		while(i!=I.end()){
			if (*i>E[e_i]) break;
			if (*i==E[e_i]){
				++i;
				break;
			}
			O.push_back(*i++);//++i;}
		}
	}
	while (i!=I.end()) O.push_back(*i++);//++i;
	return O;
}

int main(int argc, char *argv[]){
	if (strlen(argv[1])<6){
		cout << "Need to specify all includes and excludes\n";
		return 1;
	}
	int c_len = 0;
	bool inc[6] = {};
	char lo[]= "abcdefghijklmnopqrstuvwxyz";
	char up[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char dt[]= "0123456789";
	char s1[]= "!#$%&()*+-=@^_`~";
	char s2[]= "\"',./:;<>?[\\]{|}";
	char sp[]= " ";
	int C_L[] = {26,26,10,16,16,1};
	char *C[]= {lo,up,dt,s1,s2,sp};
	vector <char> CV;
	for (int i=0;i<6;++i){
		inc[i] = argv[1][i]=='1';
		if (inc[i]) c_len+=C_L[i];
	}
	CV.reserve(c_len);
	for (int i=0;i<6;++i){
		if (inc[i]){
			CV.insert(CV.end(),C[i],C[i]+C_L[i]);
		}
	}
	sort(CV.begin(),CV.end());
	for (int i=0;i<c_len;++i){
		cout << CV[i];
	}
	cout << '\n';
	vector<char> D = dif_a_b(CV,argv[2]);
	for (unsigned int i=0;i<D.size();++i){
		cout<<D[i];
	}
	cout << '\n';
	return 0;
}
