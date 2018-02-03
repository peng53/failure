#include "ui.h"

int main(int argc,char* argv[]){
	if (!init_viewr()){
		return 1;
	}
	nloop();
	//FilterForm::init();
	//FilterForm::show(0,25);
	//FilterForm::loop();
	//FilterForm::hide();
	//FilterForm::clean();
	cleanup();
	return 0;
}
