#include <string>

using std::string;
class Record {
	string uid;
	time_t start_time;
	time_t end_time;
	string code;
	string desc;
	Record(string &_uid, time_t ST, time_t ET, string &_code, string &_desc) : uid(_uid), start_time(ST), end_time(ET), code(_code), desc(_desc){ }
};
