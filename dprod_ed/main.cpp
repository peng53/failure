#include "ui.h"

int main(int argc,char* argv[]){
	if (!init_viewr()){
		return 1;
	}
	nloop();
	cleanup();
	return 0;
}
