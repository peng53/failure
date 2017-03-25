#ifndef MY_ST
#define MY_ST
#include "st.h"

ostream& operator<<(ostream& sout,Part& p){
	p.out(sout);
	return sout;
}
void CPart::out(ostream& sout){
	for (unsigned i=r;i>0;--i){
		sout<<*s;
		if (i!=1) sout<<*d;
	}
}
std::mt19937 RNG(time(0));
void RPart::out(ostream& sout){
	std::uniform_int_distribution<unsigned> r(0,c->length()-1);
	for (unsigned i=t;i>0;--i){
		for (unsigned n=0;n<l;++n){
			sout<< (*c)[r(RNG)];
		}
		if (i!=1) sout<<*d;
	}
}

#endif
