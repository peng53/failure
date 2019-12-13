#include "app.h"

int main(int argc, char** argv){
	App t;
	View v;
	Jso* j = JSON::Num(10);
	v.setViewItem(j);
	return 0;
}
