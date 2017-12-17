#include "db.h"
#include <ctime>

time_t to_time_t(const unsigned *d){
	/**
	 * Changes an array of unsigned ints to a time_t
	 * Where d is {month,day,year,hour,min}
	 * Any time_t returned will be in UTC time and
	 * have 0 seconds.
	 */
	time_t t = time(NULL);
	struct tm* u = localtime(&t);
	u->tm_mon = d[0]-1;
	u->tm_mday = d[1];
	u->tm_year = d[2] - 1900;
	u->tm_hour = d[3];
	u->tm_min = d[4];
	u->tm_sec = 0;
	return mktime(u);
}
size_t trimWS(const char *s,size_t m){
	/**
	 * Doesn't actually trim anything.
	 * Just finds ending indice that cuts off
	 * trailing spaces.
	 */
	for (size_t i=m-1;i>=0;--i){
		if (s[i]!=' ') return ++i;
	}
	return m;
}
Record::Record(): rnum(-1){
	uid.reserve(10);
	code.reserve(5);
	code.reserve(70);
}
Record::Record(char* cUID,char* cCODE,time_t tSTART,time_t tEND,char* cDESC):
	uid(cUID,trimWS(cUID,10)),
	code(cCODE,trimWS(cCODE,5)),
	desc(cDESC,trimWS(cDESC,70)),
	ds(tSTART),
	de(tEND),
	rnum(-1){
}
Record::Record(sqlite3_stmt* s):
	rnum(sqlite3_column_int(s,0)),
	uid(reinterpret_cast<const char*>(sqlite3_column_text(s,1)),10),
	code(reinterpret_cast<const char*>(sqlite3_column_text(s,2)),5),
	ds(sqlite3_column_int(s,3)),
	de(sqlite3_column_int(s,4)),
	desc(reinterpret_cast<const char*>(sqlite3_column_text(s,5)),70){
}


int open_sqdb(sqlite3** db,char *s,bool mknew){
	/**
	 * Opens a database with filename s.
	 * If mknew is true, then creates a new file (no overwrite)
	 * Otherwise, open an existing file (will not create new file)
	 */
	return sqlite3_open_v2(s,db,(mknew) ? SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL : SQLITE_OPEN_READWRITE,NULL);
}

SQLi::SQLi(sqlite3* _db,bool mknew): db(_db){
	//< Where _db is confirmed to be valid
	sqlite3_prepare_v2(db,"SELECT rowid,uid,code,start_time,end_time FROM records WHERE rowid>?1 ORDER BY rowid LIMIT ?2",-1,&vpg,0);
	sqlite3_prepare_v2(db,"INSERT INTO records (uid,start_time,end_time,code,desc) VALUES(?1,?2,?3,?4,?5)",-1,&ins,0);
	sqlite3_prepare_v2(db,"DELETE FROM records WHERE rowid=?1 LIMIT 1",-1,&del,0);
	sqlite3_prepare_v2(db,"SELECT rowid,uid,code,start_time,end_time,desc FROM records WHERE rowid=?1 LIMIT 1",-1,&getr,0);
	sqlite3_prepare_v2(db,"UPDATE records SET uid=?1, start_time=?2, end_time=?3, code=?4, desc=?5 WHERE rowid=?6",-1,&upd,0);
	sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
	if (mknew) def_table();
}
SQLi::~SQLi(){
	sqlite3_finalize(vpg);
	sqlite3_finalize(ins);
	sqlite3_finalize(del);
	sqlite3_finalize(getr);
	sqlite3_finalize(upd);
	sqlite3_close(db);
}
int SQLi::endbeg(){
	return sqlite3_exec(db,"END TRANSACTION;BEGIN TRANSACTION;",NULL,NULL,NULL);
}
int SQLi::def_table(){
	return sqlite3_exec(db,"CREATE TABLE records(uid TEXT,start_time INTEGER,end_time INTEGER,code TEXT,desc TEXT)",NULL,NULL,NULL);
}

void SQLi::bind_all(sqlite3_stmt* s,const Record &t){
	sqlite3_reset(s);
	sqlite3_bind_text(s,1,t.uid.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_int(s,2,t.ds);
	sqlite3_bind_int(s,3,t.de);
	sqlite3_bind_text(s,4,t.code.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(s,5,t.desc.c_str(),-1,SQLITE_TRANSIENT);
}
int SQLi::ins_row(const Record &t){
	bind_all(ins,t);
	return sqlite3_step(ins);
}
int SQLi::upd_row(const Record &t){
	bind_all(upd,t);
	sqlite3_bind_int(upd,6,t.rnum);
	return sqlite3_step(upd);
}
int SQLi::del_row(const unsigned rnum){
	sqlite3_reset(del);
	sqlite3_bind_int(del,1,rnum);
	return sqlite3_step(del);
}
Record SQLi::get_row(const unsigned rnum){
	sqlite3_reset(getr);
	sqlite3_bind_int(getr,1,rnum);
	sqlite3_step(getr);
	return Record(getr);
}
Record& SQLi::get_row(const unsigned rnum,Record &R){
	sqlite3_reset(getr);
	sqlite3_bind_int(getr,1,rnum);
	sqlite3_step(getr);
	R.rnum = rnum;
	R.uid = string(reinterpret_cast<const char*>(sqlite3_column_text(getr,1)),10);
	R.code = string(reinterpret_cast<const char*>(sqlite3_column_text(getr,2)),5);
	R.ds = sqlite3_column_int(getr,3);
	R.de = sqlite3_column_int(getr,4);
	R.desc = string(reinterpret_cast<const char*>(sqlite3_column_text(getr,5)),70);
	return R;
}
