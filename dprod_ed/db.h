#ifndef D_DB
#define D_DB
#include <string>
#include <ostream>
#include <sqlite3.h>

using std::string;
using std::ostream;

time_t to_time_t(unsigned *d);
size_t trimWS(char *s,size_t m);
struct Record {
	string uid;
	string code;
	string desc;
	time_t ds, de;
	int rnum;
	Record(){
		uid.reserve(10);
		code.reserve(5);
		rnum = -1;
	}
	Record(char* cUID,size_t lUID,char* cCODE,size_t lCODE,time_t tSTART,time_t tEND,char* cDESC,size_t lDESC): uid(cUID,trimWS(cUID,lUID)), code(cCODE,trimWS(cCODE,lCODE)), desc(cDESC,trimWS(cDESC,lDESC)), ds(tSTART), de(tEND){
		rnum = -1;
	}
	Record(char* cUID,char* cCODE,time_t tSTART,time_t tEND,char* cDESC): uid(cUID,trimWS(cUID,10)), code(cCODE,trimWS(cCODE,5)), desc(cDESC,trimWS(cDESC,70)), ds(tSTART), de(tEND){
		rnum = -1;
	}
	Record(sqlite3* db,int n);
};

ostream& operator<<(ostream& OUT,Record& R);
int open_exdb(sqlite3** db,char *s);
int open_nwdb(sqlite3** db,char *s);
void def_table(sqlite3 *db);
void ins_table(sqlite3 *db, Record &t);

#endif
