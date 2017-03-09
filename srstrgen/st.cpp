#ifndef MY_ST
#define MY_ST
#include "st.h"

ostream& operator<<(ostream& out,ST &P){
	if (P.W==0){
		out << (coin(RNG) ? (*P.I) : (*P.D));
	} else if (P.L!=0){
		std::uniform_int_distribution<unsigned> r(0,(*P.I).length()-1);
		for (unsigned w=P.W;w>0;--w){
			for (unsigned l=0;l<P.L;++l) out<<(*P.I)[r(RNG)];
			if (w!=1) out<<*P.D;
		}
	} else {
		for (unsigned w=P.W;w>0;--w){
			out<<*P.I;
			if (w!=1) out<<*P.D;
		}
	}
	return out;
}
void PartedString::add_part(const string &s){
			emerg.push(new string {s});
			//cout<< "made " << emerg.top()<<'\n';
			parts.emplace_back(*emerg.top());
		}
void PartedString::add_part(unsigned I){
	parts.emplace_back(literals[I]);
}
void PartedString::add_part(unsigned I, unsigned L){
	parts.emplace_back(literals[I],L);
}
void PartedString::add_part(unsigned I, unsigned D, unsigned R){
	parts.emplace_back(literals[I],literals[D],R);
}
void PartedString::add_part(unsigned I, unsigned L, unsigned D, unsigned W){
	parts.emplace_back(literals[I],L,literals[D],W);
}
ostream& operator<<(ostream& out,PartedString &PS){
	for (auto p : PS.parts) out << p;
	return out;
}
//const char* stir_mix(string &C, const unsigned L, const unsigned W, const char *S){
	//string sS=S;
	//ST p(C,L,sS,W);
	//std::stringstream sout;
	//sout << p;
	//const string rs = sout.str();
	//const char* rs2 = rs.c_str();
	//return rs2;
//}
//extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S){
	//string sC=C;
	//return stir_mix(sC,L,W,S);
//}
//extern "C" const char* plen(const char *I,char *E, const unsigned L, const unsigned W, const char *S){
	//string CV=b_incs(I);
	//string C=remove_excs(CV,E);
	//return stir_mix(C,L,W,S);
//}

#endif
