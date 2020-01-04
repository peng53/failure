#include "app.h"

#include <string>

App::App(): width(0), height(0), viewwin(nullptr){
}

App::~App(){
	if (viewwin){
		delwin(viewwin);
	}
}

void App::setRootViewItem(View& item){
	rootViewItem = item;
	if (views.size()>0){
		views = std::stack<View>();
	}
	views.push(item);
}

void App::setDimensions(unsigned _width, unsigned _height){
	width = _width;
	height = _height;
	viewwin = newwin(_height, _width, 0, 0);
	refresh();
}

void App::drawString(const string& s){
	mvwprintw(viewwin, 1, 1, s.c_str());
}

void App::draw(){
	if (width==0 || height==0 || views.size()==0){
		return;
	}
	unsigned l = 1;
	View& v = views.top();
	while (v.state == PageState::MORE){
		move(l, 1);
		v.getItem([] (string& s){ printw(s.c_str());});
		++l;
	}
	//std::string t = "hello world!";
	//mvwprintw(viewwin,1,1,t.c_str());
	box(viewwin, 0, 0);
	wrefresh(viewwin);
}
