#ifndef D_DB
#define D_DB
#include <string>
#include <sqlite3.h>

using std::string;

time_t to_time_t(const unsigned *d); //< Returns a time_t from an array of 5 numbers of: month, day, year, hour, minute
size_t trimWS(const char* const s,const size_t m); //< Return length of char* without trailing spaces
struct Record {
	/**
	 * Contains data for an entry for use in database.
	 */
	string uid, code, desc;
	time_t ds, de;
	int rnum;
	Record(); //< Inits a Record with no data
	Record(const char* const fUID,const char* const fCODE,const time_t fTSTART,const time_t fTEND); //< Inits a 'condensed' Record. For use with FilterForm
	Record(const char* const cUID,const char* const cCODE,const time_t tSTART,const time_t tEND,const char* const cDESC); //< Inits a Record with all data values
	Record(sqlite3_stmt* s); //< Inits a Record directly from database
};

class SQLi {
	/**
	 * A wrapper/adaptor class around a SQLite object.
	 */
	private:
		sqlite3* db;
		sqlite3_stmt *ins,*del,*getr,*upd;
		void bind_all(sqlite3_stmt* s,const Record&); //< Binds a Record's values to a SQL statement.
	public:
		sqlite3_stmt *vpg;
		SQLi(sqlite3* _db); //< Inits a SQLi object with base sqlite3 object.
		~SQLi(); //< SQLi destructor
		int endbeg(); //< Saves changes to file
		int chg_row(const Record&,const bool new1_existing0); //< Modifies an entry in database with match Record obj
		void set_vpg(); //< Resets page-viewer to 'normal'
		void set_cus(const string& str,const Record,const bool has_st,const bool has_et); //< Sets page-viewer to only show entries matching filtering Record
		int del_row(const unsigned rnum); //< Deletes an entry
		Record get_row(const unsigned rnum); //< Returns Record object for an entry
		Record& get_row(const unsigned rnum,Record&); // Modifies Record object to match an entry
};
int open_sqdb(sqlite3** db,const char* const filename,const bool new1_existing0); //< Tries to open a file as sqlite3 database
#endif
