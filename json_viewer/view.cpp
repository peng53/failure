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
	item = 0;
	page = 0;
	state = PageState::MORE;
}

void View::setItemsPerPage(unsigned count){
	itemsPerPage = count;
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
	if (state != PageState::MORE){
		return;
	}
	string out;
	if (item < itemsPerPage){
		switch (displayedItem->t){
			case JType::Num:
				out = JsoStringRep(displayedItem);
				state = PageState::DONE;
				break;
			case JType::Str:
				out = JsoStringRep(displayedItem);
				state = PageState::DONE;
				break;
			case JType::Arr:
				if (item < itemsPerPage){
					size_t index = item + itemsPerPage*page;
					if (index >= displayedItem->x.a->size()){
						state = PageState::DONE;
					} else {
						out = JsoStringRep((*displayedItem->x.a)[index]);
						++item;
					}
				}
				break;
			case JType::Obj:
				if (item < itemsPerPage){
					size_t index = item + itemsPerPage*page;
					if (index >= displayedItem->x.m->size()){
						state = PageState::DONE;
					} else {
						unordered_map<string,Jso*>::iterator it = displayedItem->x.m->begin();
						for (size_t i = 0; i < index; ++i){
							++it;
						}
						out = it->first;
						++item;
					}
				}
				break;
			default:
				break;
		}
		f(out);
	}
}

void View::reloadPage(){
	state = PageState::MORE;
	item = 0;
}
