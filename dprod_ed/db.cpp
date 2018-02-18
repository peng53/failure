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
size_t trimWS(const char* const s,const size_t m){
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
Record::Record(const char* const fUID,const char* const fCODE,const time_t fTSTART,const time_t fTEND):
	uid(fUID,trimWS(fUID,10)),
	code(fCODE,trimWS(fCODE,5)),
	ds(fTSTART),
	de(fTEND){
}
Record::Record(const char* const cUID,const char* const cCODE,const time_t tSTART,const time_t tEND,const char* const cDESC):
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

int open_sqdb(sqlite3** db,const char* const s,const bool mknew){
	/**
	 * Opens a database with filename s.
	 * If mknew is true, then creates a new file (no overwrite)
	 * Otherwise, open an existing file (will not create new file)
	 * Checks by inserting row. (and rolling-back)
	 */
	int r = sqlite3_open_v2(s,db,((mknew) ? (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) : SQLITE_OPEN_READWRITE),NULL);
	if (r==0){
		if (mknew){
			r = sqlite3_exec(*db,"CREATE TABLE records(uid TEXT,start_time INTEGER,end_time INTEGER,code TEXT,desc TEXT)",NULL,NULL,NULL);
		} else {
			r = sqlite3_exec(*db,"BEGIN TRANSACTION;INSERT INTO records (uid,start_time,end_time,code,desc) VALUES('u',0,9,'c','d')",NULL,NULL,NULL);
			sqlite3_exec(*db,"ROLLBACK",NULL,NULL,NULL);
		}
	}
	return r;
}

static const string sel_rows = "SELECT rowid,uid,code,start_time,end_time FROM records WHERE rowid>?1";
static const string row_order = " ORDER BY rowid LIMIT ?2";

SQLi::SQLi(sqlite3* _db): db(_db){
	//< Where _db is confirmed to be valid
	sqlite3_prepare_v2(db,string(sel_rows+row_order).c_str(),-1,&vpg,0);
	sqlite3_prepare_v2(db,"INSERT INTO records (uid,start_time,end_time,code,desc) VALUES(?1,?2,?3,?4,?5)",-1,&ins,0);
	sqlite3_prepare_v2(db,"DELETE FROM records WHERE rowid=?1 LIMIT 1",-1,&del,0);
	sqlite3_prepare_v2(db,"SELECT rowid,uid,code,start_time,end_time,desc FROM records WHERE rowid=?1 LIMIT 1",-1,&getr,0);
	sqlite3_prepare_v2(db,"UPDATE records SET uid=?1, start_time=?2, end_time=?3, code=?4, desc=?5 WHERE rowid=?6",-1,&upd,0);
	sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
}
SQLi::~SQLi(){
	sqlite3_finalize(vpg);
	sqlite3_finalize(ins);
	sqlite3_finalize(del);
	sqlite3_finalize(getr);
	sqlite3_finalize(upd);
	sqlite3_close(db);
}
void SQLi::set_vpg(){
	sqlite3_finalize(vpg);
	sqlite3_prepare_v2(db,string(sel_rows+row_order).c_str(),-1,&vpg,0);
}
void SQLi::set_cus(const string& str,const Record t,const bool st_eq,const bool et_eq){
	sqlite3_finalize(vpg);
	sqlite3_prepare_v2(db,string(sel_rows+str+row_order).c_str(),-1,&vpg,0);
	// TRANSIENT is needed because the Record isn't stored.
	if (t.uid.length()>0){
		sqlite3_bind_text(vpg,3,t.uid.c_str(),-1,SQLITE_TRANSIENT);
	}
	if (st_eq){
		sqlite3_bind_int(vpg,5,t.ds);
	}
	if (et_eq){
		sqlite3_bind_int(vpg,6,t.de);
	}
	if (t.code.length()>0){
		sqlite3_bind_text(vpg,4,t.code.c_str(),-1,SQLITE_TRANSIENT);
	}
}
int SQLi::endbeg(){
	return sqlite3_exec(db,"END TRANSACTION;BEGIN TRANSACTION;",NULL,NULL,NULL);
}
void SQLi::bind_all(sqlite3_stmt* s,const Record &t){
	sqlite3_reset(s); //SQLITE_TRANSIENT
	sqlite3_bind_text(s,1,t.uid.c_str(),-1,SQLITE_STATIC);
	sqlite3_bind_int(s,2,t.ds);
	sqlite3_bind_int(s,3,t.de);
	sqlite3_bind_text(s,4,t.code.c_str(),-1,SQLITE_STATIC);
	sqlite3_bind_text(s,5,t.desc.c_str(),-1,SQLITE_STATIC);
}
int SQLi::chg_row(const Record &t,const bool mknew){
	sqlite3_stmt* s = (mknew) ? ins : upd;
	bind_all(s,t);
	if (!mknew) sqlite3_bind_int(s,6,t.rnum);
	return sqlite3_step(s);
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
	R.uid.assign(reinterpret_cast<const char*>(sqlite3_column_text(getr,1)),10);
	R.code.assign(reinterpret_cast<const char*>(sqlite3_column_text(getr,2)),5);
	R.ds = sqlite3_column_int(getr,3);
	R.de = sqlite3_column_int(getr,4);
	R.desc.assign(reinterpret_cast<const char*>(sqlite3_column_text(getr,5)),70);
	return R;
}
