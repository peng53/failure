#include "view.h"
#include <string>

using std::to_string;

View::View(): page(0), item(0){

}

View::~View(){

}

void View::setViewItem(Jso* item){
	displayedItem = item;
}

void View::setItemsPerPage(unsigned count){
	itemsPerPage = count;
}

void View::getItem(void (*f) (const char* s)){
	string out;
	if (item < itemsPerPage){
		switch (displayedItem->t){
			case JType::Num:
				out += to_string(displayedItem->x.f);
				break;
			default:
				break;
		}
		f(out.c_str());
	}
}
