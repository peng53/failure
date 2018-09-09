#include "b_inc.h"

static const string C[] = {
	"abcdefghijklmnopqrstuvwxyz",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	"0123456789",
	"!#$%&*+-=@^_`~",
	"\"',./:;?\\|",
	" ()<>[]{}"
};

BInc::BInc(const string& I){
	size_t c_len = 0;
	for (size_t i=0;i<6;++i){
		if (I[i]=='1') c_len += C[i].length();
	}
	if (c_len!=0){
		s.reserve(c_len);
		for (size_t i=0;i<6;++i){
			if (I[i]=='1') s += C[i];
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
