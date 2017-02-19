#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]){
	if (strlen(argv[2])<6){
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
		inc[i] = argv[2][i]=='1';
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
	return 0;
}
