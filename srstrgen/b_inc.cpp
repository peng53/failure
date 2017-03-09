#ifndef B_INC
#define B_INC
#include "b_inc.h"

string b_incs(const char* I){
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

#endif
