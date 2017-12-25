#include "b_inc.h"

static const char *C[]= {
	"abcdefghijklmnopqrstuvwxyz",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	"0123456789",
	"!#$%&*+-=@^_`~",
	"\"',./:;?\\|",
	" ()<>[]{}"
};
static const unsigned C_L[] = {26,26,10,14,11,9};

BInc::BInc(const char* I){
	unsigned c_len = 0;
	for (unsigned i=0;i<6;++i){
		if (I[i]=='1') c_len += C_L[i];
	}
	if (c_len==0){
		s = " ";
	} else {
		s.reserve(c_len);
		for (unsigned i=0;i<6;++i){
			if (I[i]=='1') s.insert(s.end(),C[i],C[i]+C_L[i]);
		}
	}
}
void BInc::rem(char* E){
	sort(s.begin(),s.end());
	string O;
	O.reserve(s.length());
	auto i = s.cbegin();
	auto e_len = strlen(E);
	sort(E,E+e_len);
	for (char *e=E;e!=E+e_len;++e){
		if (*i<=*e){
			for (;i!=s.cend();O.push_back(*i++)){
				if (*i==*e){ ++i; break; }
			}
		}
	}
	O.insert(O.end(),i,s.cend());
	s = O;
}
void BInc::add(const char* A){
	unsigned a_len = strlen(A);
	s.reserve(s.length()+a_len);
	s.insert(s.end(),A,A+a_len);
}
