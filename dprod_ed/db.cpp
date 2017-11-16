#ifndef D_DB
#define D_DB
#include "db.h"

void TRecord::print(){
	printf("UID : %s\nCODE: %s\nDATE:\n  START: %02d/%02d/%04d - %02d:%02d\n  END  : %02d/%02d/%04d - %02d:%02d\n",
		uid,code,date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7],date[8],date[9]);
}
void TRecord::qfix(){
	for (int i=0;i<2;++i){
		date[i*5] %= 13;
		if (date[i*5]==0){
			date[i*5]=12;
			date[1+i*5]-=1;
		}
		date[1+i*5] %= 32;
		date[3+i*5] %= 24;
		date[4+i*5] %= 60;
	}
}
#endif