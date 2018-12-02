#include <sqlite3.h>

int main(){
	sqlite3 **ppDb;
	sqlite3_open_readonly("test.db",ppDb);
	sqlite3_stmt *ppStmt;
	sqlite3_prepare_v2(ppDb,"SELECT * from prod_records",-1,*ppStmt,0);
	return 0;
	}
