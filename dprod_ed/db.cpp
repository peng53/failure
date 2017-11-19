#ifndef D_DB
#define D_DB
#include "db.h"
#include <sqlite3.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <ostream>
#include <string>

using std::ostream;
using std::cout;
using std::setw;
using std::setfill;
using std::string;

time_t to_time_t(unsigned *d){
	time_t t = time(NULL);
	struct tm* u = gmtime(&t);
	u->tm_mon = d[0]-1;
	u->tm_mday = d[1];
	u->tm_year = d[2] - 1900;
	u->tm_hour = d[3];
	u->tm_min = d[4];
	u->tm_sec = 0;
	return mktime(u);
}

struct Record {
	string uid,code,desc;
	time_t ds, de;
	void out();
	void append_desc(char *s);
};

ostream& operator<<(ostream& OUT,Record& R){
	struct tm* d = gmtime(&R.ds);
	OUT << R.uid << '\n';
	OUT << R.code << '\n';
	OUT <<d->tm_mon+1<<'/'<<setw(2)<<d->tm_mday<<'/'<<setw(4)<< d->tm_year+1900<<" - "<< setw(2)<<d->tm_hour<<':'<<setw(2)<<d->tm_min<<'\n';
	OUT << R.desc << '\n';
	return OUT;
}
void Record::append_desc(char *s){
	desc += '`';
	desc += s;
}

int open_exdb(sqlite3** db,char *s){
	int r = sqlite3_open_v2(s,db,SQLITE_OPEN_READWRITE,NULL);
	if (r==0) return 0;
	return 1;
}
int open_nwdb(sqlite3** db,char *s){
	int r = sqlite3_open_v2(s,db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);
	if (r==0) return 0;
	return 1;
}
void def_table(sqlite3 *db){
	//~ sqlite3_stmt* s;
	//~ sqlite3_prepare_v2(db,"CREATE TABLE records(uid TEXT,start_time INTEGER,end_time INTEGER,code TEXT,desc TEXT)",-1,&s,0);
	sqlite3_exec(db,"CREATE TABLE records(uid TEXT,start_time INTEGER,end_time INTEGER,code TEXT,desc TEXT)",NULL,NULL,NULL);
}
void ins_table(sqlite3 *db, Record &t){
	sqlite3_stmt* s;
	sqlite3_prepare_v2(db,"INSERT INTO records (uid,start_time,end_time,code,desc) VALUES(?1,?2,?3,?4,?5)",-1,&s,0);
	sqlite3_bind_text(s,1,t.uid.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(s,4,"PLACEHOLDER",-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(s,5,t.desc.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_int(s,2,t.ds);
	sqlite3_bind_int(s,3,t.de);
	sqlite3_step(s);
	sqlite3_finalize(s);
}


#endif