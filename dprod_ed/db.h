#ifndef D_DB
#define D_DB
#include <string>
#include <sqlite3.h>

using std::string;

time_t to_time_t(const unsigned *d);
size_t trimWS(const char *s,size_t m);
struct Record {
	string uid;
	string code;
	string desc;
	time_t ds, de;
	int rnum;
	Record();
	Record(char* cUID,char* cCODE,time_t tSTART,time_t tEND,char* cDESC);
	Record(sqlite3_stmt* s);
};

struct SQLi {
	sqlite3* db;
	sqlite3_stmt *vpg,*ins,*del,*getr,*upd;
	SQLi(sqlite3* _db);
	~SQLi();
	int endbeg();
	int def_table();
	void bind_all(sqlite3_stmt* s,const Record &t);
	int ins_row(const Record &t);
	int del_row(const unsigned rnum);
	int upd_row(const Record &t);
	Record get_row(const unsigned rnum);
	Record& get_row(const unsigned rnum,Record &R);
};
int open_exdb(sqlite3** db,char *s);
int open_nwdb(sqlite3** db,char *s);
#endif
