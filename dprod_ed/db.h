#ifndef D_DB
#define D_DB
#include <string>
#include <sqlite3.h>

using std::string;

time_t to_time_t(const unsigned *d);
size_t trimWS(const char* const s,const size_t m);
struct Record {
	string uid, code, desc;
	time_t ds, de;
	int rnum;
	Record();
	Record(const char* const fUID,const char* const fCODE,const time_t fTSTART,const time_t fTEND);
	Record(const char* const cUID,const char* const cCODE,const time_t tSTART,const time_t tEND,const char* const cDESC);
	Record(sqlite3_stmt* s);
};

class SQLi {
	private:
		sqlite3* db;
		sqlite3_stmt *ins,*del,*getr,*upd;
		void bind_all(sqlite3_stmt* s,const Record&);
	public:
		sqlite3_stmt *vpg;
		SQLi(sqlite3* _db);
		~SQLi();
		int endbeg();
		int chg_row(const Record&,const bool new1_existing0);
		void set_vpg();
		void set_cus(const string& str,const Record,const bool has_st,const bool has_et);
		int del_row(const unsigned rnum);
		Record get_row(const unsigned rnum);
		Record& get_row(const unsigned rnum,Record&);
};
int open_sqdb(sqlite3** db,const char* const s,const bool new1_existing0);
#endif
