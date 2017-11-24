#include "db.h"
#include <iomanip>
#include <ctime>

using std::setw;
using std::setfill;

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
size_t trimWS(char *s,size_t m){
	for (size_t i=m-1;i>=0;--i){
		if (s[i]!=' ') return ++i;
	}
	return m;
}

Record::Record(sqlite3* db,int n){
	sqlite3_stmt* s;
	sqlite3_prepare_v2(db,"SELECT uid,start_time,end_time,code,desc FROM records WHERE rowid==?1",-1,&s,0);
	sqlite3_bind_int(s,1,n);
	if (sqlite3_step(s)!=SQLITE_DONE){
		uid = string(reinterpret_cast<const char*>(sqlite3_column_text(s,0)),10);
		code = string(reinterpret_cast<const char*>(sqlite3_column_text(s,3)),5);
		desc = string(reinterpret_cast<const char*>(sqlite3_column_text(s,4)),70);
		ds = sqlite3_column_int(s,1);
		de = sqlite3_column_int(s,2);
		rnum = n;
	}
}

ostream& operator<<(ostream& OUT,Record& R){
	OUT << '\"' << R.uid <<'\"' << '\n' <<'\"' << R.code <<'\"' << '\n';
	struct tm* d = gmtime(&R.ds);
	OUT<<setfill('0')<<setw(2)<<d->tm_mon+1<<'/'<<setw(2)<<d->tm_mday<<'/'<<setw(2)<<(d->tm_year)%100<<" - "<<setw(2)<<d->tm_hour<<':'<<setw(2)<<d->tm_min<<'\n';
	d = gmtime(&R.de);
	OUT<<setfill('0')<<setw(2)<<d->tm_mon+1<<'/'<<setw(2)<<d->tm_mday<<'/'<<setw(2)<<(d->tm_year)%100<<" - "<<setw(2)<<d->tm_hour<<':'<<setw(2)<<d->tm_min<<'\n';
	OUT <<'\"' << R.desc <<'\"' << '\n';
	return OUT;
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
	sqlite3_exec(db,"CREATE TABLE records(uid TEXT,start_time INTEGER,end_time INTEGER,code TEXT,desc TEXT)",NULL,NULL,NULL);
}
void ins_table(sqlite3 *db, Record &t){
	sqlite3_stmt* s;
	sqlite3_prepare_v2(db,"INSERT INTO records (uid,start_time,end_time,code,desc) VALUES(?1,?2,?3,?4,?5)",-1,&s,0);
	sqlite3_bind_text(s,1,t.uid.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(s,4,t.code.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_text(s,5,t.desc.c_str(),-1,SQLITE_TRANSIENT);
	sqlite3_bind_int(s,2,t.ds);
	sqlite3_bind_int(s,3,t.de);
	sqlite3_step(s);
	sqlite3_finalize(s);
}
