#include "view.h"
#include <string>
#include <iostream>

using std::to_string;
using std::string;

View::View(): page(0), item(0){

}

View::~View(){

}

void View::setViewItem(Jso* viewItem){
	displayedItem = viewItem;
	firstPage();
}

void View::setItemsPerPage(unsigned count){
	itemsPerPage = count;
	firstPage();
}

const string JsoStringRep(Jso *j){
	switch (j->t){
		case JType::Num:
			return to_string(j->x.f);
		case JType::Str:
			return string(*(j->x.s));
		case JType::Arr:
			return "[array]";
		case JType::Obj:
			return "{obj}";
		case JType::Null:
			return "(null)";
		case JType::True:
			return "(true)";
		case JType::False:
			return "(false)";
		default:
			return "SHOULD NOT BE SEEN JSOSTRINGREP";
	}
}

const string subitemDist(Jso* j){
	switch (j->t){
		case JType::Arr:
			return " ~[]";
		case JType::Obj:
			return " ~{}";
		default:
			return "";
	}
}

void View::getItem(void (*f) (string& s)){
	if (state != PageState::MORE || item >= itemsPerPage){
		state = PageState::DONE;
		return;
	}
	string out;
	size_t index = item + itemsPerPage*page;
	switch (displayedItem->t){
		case JType::Arr:
			if (index >= displayedItem->x.a->size()){
				state = PageState::DONE;
			} else {
				out = JsoStringRep((*displayedItem->x.a)[index])
					+ subitemDist((*displayedItem->x.a)[index]);

				++item;
			}
			break;
		case JType::Obj:
			if (index >= displayedItem->x.m->size()){
				state = PageState::DONE;
			} else {
				out = it->first + subitemDist(it->second);
				++item;
				++it;
			}
			break;
		default:
			out = JsoStringRep(displayedItem);
			state = PageState::DONE;
			break;
	}
	if (state != PageState::DONE){
		f(out);
	}

}

void View::reloadPage(){
	state = PageState::MORE;
	item = 0;
}

void View::firstPage(){
	reloadPage();
	page = 0;
	if (displayedItem->t == JType::Obj){
		it = displayedItem->x.m->begin();
	}
}

bool View::nextPage(){
	// first check if new page is valid
	size_t items = 0;
	switch (displayedItem->t){
		case JType::Arr:
			items = displayedItem->x.a->size();
			break;
		case JType::Obj:
			items = displayedItem->x.m->size();
			break;
		default:
			break;
	}
	if ((page+1)*itemsPerPage >= items){
		return false;
		// currently on last page
	}
	state = PageState::MORE;
	++page;
	// now to increment iterator to first item of new page
	if (displayedItem->t == JType::Obj){
		// it could already be there
		std::advance(it, itemsPerPage-item);
	}
	item = 0;
	return true;
}

