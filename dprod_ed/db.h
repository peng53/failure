#ifndef D_DB
#define D_DB
#include <string>
#include <ostream>
#include <sqlite3.h>

using std::string;
using std::ostream;

time_t to_time_t(const unsigned *d);
size_t trimWS(const char *s,size_t m);
struct Record {
	string uid;
	string code;
	string desc;
	time_t ds, de;
	int rnum;
	Record();
	Record(char* cUID,size_t lUID,char* cCODE,size_t lCODE,time_t tSTART,time_t tEND,char* cDESC,size_t lDESC);
	Record(char* cUID,char* cCODE,time_t tSTART,time_t tEND,char* cDESC);
	Record(sqlite3* db,int n);
};

ostream& operator<<(ostream& OUT,const Record& R);
int open_exdb(sqlite3** db,char *s);
int open_nwdb(sqlite3** db,char *s);
void def_table(sqlite3 *db);
void ins_table(sqlite3 *db,const Record &t);

#endif
