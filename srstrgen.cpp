#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

char C[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()_+-=,./;'[]<>?:\"{}|\\ ";
int C_L[] = {26,26,10,16,16,1};
int C_R2[] = {0,26,52,62,78,94};

char c_lookup(bool inc[6],int r){
	for (int i=0;i<6;++i){
		if (inc[i]){
			if (C_R2[i]<=r && r<C_R2[i]+C_L[i]){
				//cout << r << ',' << C[r] << '\n';
				return C[r];
			}
		} else r+=C_L[i];
	}
	return '\t';
}

int main(int argc, char *argv[]){
	if (argc!=3){
		cout << "Needs more args\n";
		return 1;
	}
	if (strlen(argv[2])<6){
		cout << "Need to specify all includes and excludes\n";
		return 1;
	}
	int c_len = 0;
	bool inc[6] = {};
	for (int i=0;i<6;++i){
		inc[i] = argv[2][i]=='1';
		if (inc[i]) c_len+=C_L[i];
		//if (argv[i+2][0]=='1'){
			//inc[i]=1;
			//c_len+=C_L[i];
		//} else inc[i]=0;
	}
	mt19937 rng(time(0));
	uniform_int_distribution<int> rrr(0,c_len-1);
	for (unsigned int len = atoi(argv[1]); len>0; --len){
		cout << c_lookup(inc,rrr(rng));
	}
	cout << '\n';
	//for (int i=0;i<c_len;++i){ c_lookup(inc,i); }
	return 0;
}
