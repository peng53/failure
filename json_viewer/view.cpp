#include "view.h"
#include <string>

using std::to_string;
using std::string;

View::View(): page(0), item(0){

}

View::~View(){

}

void View::setViewItem(Jso* viewItem){
	displayedItem = viewItem;
	if (displayedItem->t == JType::Obj){
		it = displayedItem->x.m->begin();
	}
	item = 0;
	page = 0;
	state = PageState::MORE;
}

void View::setItemsPerPage(unsigned count){
	itemsPerPage = count;
	if (displayedItem->t == JType::Obj){
		it = displayedItem->x.m->begin();
	}
	item = 0;
	page = 0;
	state = PageState::MORE;
}

string JsoStringRep(Jso *j){
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

void View::getItem(void (*f) (string& s)){
	if (state != PageState::MORE || item >= itemsPerPage){
		return;
	}
	string out;
	size_t index = item + itemsPerPage*page;
	switch (displayedItem->t){
		case JType::Arr:
			if (index >= displayedItem->x.a->size()){
				state = PageState::DONE;
			} else {
				out = JsoStringRep((*displayedItem->x.a)[index]);
				++item;
			}
			break;
		case JType::Obj:
			if (index >= displayedItem->x.m->size()){
				state = PageState::DONE;
			} else {
				out = it->first;
				++item;
				++it;
			}
			break;
		default:
			out = JsoStringRep(displayedItem);
			state = PageState::DONE;
			break;
	}
	f(out);

}

void View::reloadPage(){
	state = PageState::MORE;
	item = 0;
}
