
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;


int main(){

	union tu {
		struct half {
			char a, b, c, d;
		} f1;
		unsigned f2;
		tu(unsigned v): f2(v){}
	};
	tu f(10);
	cout << f.f2 << '\n';
	f.f1.a = 'a';
	f.f1.b = 'b';
	f.f1.c = 'c';
	f.f1.d = 'd';
	cout << f.f1.a << '\n';
	cout << "++++" << '\n';
	vector<time_t> micky;
	time_t rt;
	time(&rt);
	micky.push_back(rt);
	struct tm *k;
	k = localtime(&rt);
	k->tm_isdst = 0;

	k->tm_sec = 0;
	k->tm_min = 10;
	k->tm_hour = 6;
	k->tm_mday = 25;
	k->tm_mon = 0;
	k->tm_year = 1994-1900;
	rt = mktime(k);
	micky.push_back(rt);

	k->tm_sec = 10;
	k->tm_min = 10;
	k->tm_hour = 6;
	k->tm_mday = 25;
	k->tm_mon = 0;
	k->tm_year = 1994-1900;
	rt = mktime(k);
	micky.push_back(rt);


	cout << k->tm_wday << '\n';
	for (auto T : micky){
		cout << T << '\n';
	}
	cout << difftime(micky[2],micky[1]) << '\n';

	return 0;

}
