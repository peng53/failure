#include "app.h"

#include <string>

App::App(): width(0), height(0), selectedLine(0), _running(true){
}


void App::setRootViewItem(View& item){
	if (views.size()>0){
		views = std::stack<View>();
	}
	views.push(item);
}

void App::setDimensions(unsigned _width, unsigned _height){
	width = _width;
	height = _height;
}

void App::draw(){
	if (width==0 || height==0 || views.size()==0){
		return;
	}
	selectedLine = 0;
	View& v = views.top();
	clear();
	for (unsigned l=0; v.state == PageState::MORE && l<height; ++l){
		move(l, 1);
		v.getItem([](const string& s){ printw(s.c_str());});
	}
	move(0,0);
	refresh();
}

void App::keySym(int c){
	if (keybinds.count(c)==1){
		switch (keybinds[c]){
			case Command::NEXTPG:
				nextPage();
				break;
			case Command::PREVPG:
				prevPage();
				break;
			case Command::CLOSEVIEW:
				closeView();
				break;
			case Command::OPENVIEW:
				openSelectedItem();
				break;
			case Command::MOVEUP:
				selectLowerItem();
				break;
			case Command::MOVEDOWN:
				selectUpperItem();
				break;
			case Command::RELOAD:
				reload();
				break;
			case Command::QUIT:
				quit();
				break;
		}
	}
}

void App::selectUpperItem(){
	if (selectedLine+1<height){
		++selectedLine;
		move(selectedLine,0);
	}
}
void App::selectLowerItem(){
	if (selectedLine>0){
		--selectedLine;
		move(selectedLine,0);
	}
}

void App::reload(){
	views.top().reloadPage();
	selectedLine = 0;
	draw();
}

void App::closeView(){
	if (views.size()>1){
		views.pop();
		reload();
	}
}

void App::openSelectedItem(){
	View newview = views.top().openNthItem(selectedLine);
	views.push(newview);
	draw();
}

void App::prevPage(){
	if (views.top().prevPage()){
		draw();
	}
}
void App::nextPage(){
	if (views.top().nextPage()){
		draw();
	}
}


void App::quit(){
	_running = false;
}

void App::addBind(int k, Command cmd){
	if (keybinds.count(k)==0){
		keybinds.emplace(k,cmd);
	}
}

bool App::running(){
	return _running;
}
