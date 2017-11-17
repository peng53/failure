#include <string>
#include <cstdio>

using std::string;

struct TRecord {
	char uid[11] = "\0";
	char code[6] = "\0";
	unsigned int date[10];
	char desc[76] = {'\0'};
	void print();
	void qfix();
};

class Record {
	string uid;
	time_t start_time;
	time_t end_time;
	string code;
	string desc;
	Record(string &_uid, time_t ST, time_t ET, string &_code, string &_desc) : uid(_uid), start_time(ST), end_time(ET), code(_code), desc(_desc){ }
};
