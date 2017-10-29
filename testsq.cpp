#include <cstdio>
#include <sqlite3.h>
#include <ctime>

void pretty_date(time_t s){
	struct tm * X = localtime(&s);
	printf("%02d/%02d/%4d %02d:%02d", X->tm_mon,X->tm_mday,X->tm_year+1900,X->tm_hour,X->tm_min);
}

int main(){
	sqlite3* db;
	sqlite3_stmt* s;
	sqlite3_open("dates.db",&db);
	sqlite3_prepare_v2(db,"SELECT * from events",-1,&s,0);
	for (int n = 0; sqlite3_step(s)==100 && n<25; ++n){
		printf("%02d| ", sqlite3_column_int(s,0));
		pretty_date(sqlite3_column_int(s,1));
		fputs(" - ", stdout);
		pretty_date(sqlite3_column_int(s,2));
		printf(" | %-25s / %s\n",sqlite3_column_text(s,3),sqlite3_column_text(s,4));
	}
	sqlite3_finalize(s);
	sqlite3_close(db);
	return 0;
}