#include "b_inc.h"

static const char *C[]= {
	"abcdefghijklmnopqrstuvwxyz",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	"0123456789",
	"!#$%&*+-=@^_`~",
	"\"',./:;?\\|",
	" ()<>[]{}"
};
static const size_t C_L[] = {26,26,10,14,11,9};

BInc::BInc(const char* I){
	size_t c_len = 0;
	for (size_t i=0;i<6;++i){
		if (I[i]=='1') c_len += C_L[i];
	}
	if (c_len==0){
		s = " ";
	} else {
		s.reserve(c_len);
		for (size_t i=0;i<6;++i){
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
	s.assign(O);
}
void BInc::add(const string& A){
	s.reserve(s.length()+A.length());
	s += A;
}
