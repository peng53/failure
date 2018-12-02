// dbstuff.cpp
#include <iostream>
#include <ctime>

using namespace std;

struct Record {
	string uid;
	string code;
	string desc;
	struct tm * ds;
	struct tm* de;
	Record(){
		time_t rawtime;
		time(&rawtime);
		struct tm * ds = localtime(&rawtime);
		struct tm * de = localtime(&rawtime);
	}
	void out(){
		cout << ds->tm_mon+1 << ds->tm_mday << ds->tm_year+1900 << '\n';
	}
	void set_ds(unsigned int* date){
		ds->tm_mon = date[0]-1;
		ds->tm_mday = date[1];
		ds->tm_year = date[2] - 1900;
		ds->tm_hour = date[3];
		ds->tm_min = date[4];
		ds->tm_sec = 0;
		ds->tm_isdst = 0;
	}
};

int main(){
	Record t;
	unsigned int d[5] = {10,24,2017,3,24};
	t.set_ds(d);
	t.out();
	time_t rawtime;
	time(&rawtime);
	cout<<rawtime<<'\n';

	return 0;
}