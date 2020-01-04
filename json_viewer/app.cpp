#include "app.h"

App::App(): width(0), height(0), viewwin(nullptr){
}

App::~App(){
	if (viewwin){
		delwin(viewwin);
	}
}

void App::setRootViewItem(View& item){
	rootViewItem = item;
}

void App::setDimensions(unsigned _width, unsigned _height){
	width = _width;
	height = _height;
	viewwin = newwin(_height, _width, 0, 0);
	refresh();
	box(viewwin, 0, 0);
	wrefresh(viewwin);
}
