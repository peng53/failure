#ifndef PY_EXTER
#define PY_EXTER
#include "exo.h"
const char* stir_mix(string C, const unsigned L, const unsigned W, const char *D){
	string sD = {D};
	ST p(&C,L,&sD,W);
	std::stringstream out;
	out << p;
	const string rs = out.str();
	const char* rs2 = rs.c_str();
	return rs2;
}
extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *D){
	return stir_mix(string {C},L,W,D);
}
extern "C" const char* plen(const char *I,char *E, const unsigned L, const unsigned W, const char *D){
	string CV=b_incs(I);
	return stir_mix(remove_excs(CV,E),L,W,D);
}
#endif
