#include <cstdio>
#include <sqlite3.h>
#include <ctime>

int main(){
	sqlite3 *test;
	sqlite3_stmt **s;
	printf("%d",sqlite3_open("dates.db",&test));
	printf("%d",sqlite3_prepare_v2(test,"SELECT * from events",-1,s,0));
	int t = 0;
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	//timeinfo = localtime(&rawtime);
	//printf("Current local time and date: %s", asctime(timeinfo));


	while(sqlite3_step(*s) && t<25){
		printf("\n%2d | %d | %d | ",sqlite3_column_int(*s,0),sqlite3_column_int(*s,1),sqlite3_column_int(*s,2));
		printf("%s",sqlite3_column_text(*s,3));
		t++;
	}
	//printf("%d",sqlite3_step(*s));
	//printf("\n%s\n",sqlite3_column_text(*s,3));
	printf("%d",sqlite3_finalize(*s));
	return 0;
}