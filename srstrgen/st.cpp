#ifndef MY_ST
#define MY_ST
#include "st.h"

std::mt19937 RNG(time(0));
std::binomial_distribution<bool> coin(1,0.5);

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
void PartedString::add_literal(const string &s){
	literals.emplace_back(new string {s});
	//cout<< "made " << *literals.back() << " at " << literals.back()<<'\n';
}
PartedString& PartedString::operator<<(const string &s){
	add_literal(s);
	return *this;
}
void PartedString::add_part(const string &s){
	add_literal(s);
	parts.emplace_back(literals.back());
}
void PartedString::add_part(const unsigned I){
	parts.emplace_back(literals[I]);
}
void PartedString::add_part(const unsigned I,const unsigned L){
	parts.emplace_back(literals[I],L);
}
void PartedString::add_part(const unsigned I,const unsigned D,const unsigned R){
	parts.emplace_back(literals[I],literals[D],R);
}
void PartedString::add_part(const unsigned I,const unsigned L,const unsigned D,const unsigned W){
	parts.emplace_back(literals[I],L,literals[D],W);
}
ostream& operator<<(ostream& out,PartedString &PS){
	for (auto p : PS.parts) out << p;
	return out;
}
#endif
