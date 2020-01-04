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
	switch (displayedItem->t){
		case JType::Arr:
			out = getItemFromArray();
			break;
		case JType::Obj:
			out = getItemFromObject();
			break;
		default:
			out = JsoStringRep(displayedItem);
			f(out);
			state = PageState::DONE;
			break;
	}
	if (state != PageState::DONE){
		f(out);
	}
}

string View::getItemFromArray(){
	size_t index = item + itemsPerPage*page;
	if (index >= displayedItem->x.a->size()){
		state = PageState::DONE;
		return "";
	} else {
		++item;
		return JsoStringRep((*displayedItem->x.a)[index]);
	}
}

string View::getItemFromObject(){
	size_t index = item + itemsPerPage*page;
	if (index >= displayedItem->x.m->size()){
		state = PageState::DONE;
		return "";
	} else {
		string out = mapPlaceholder->first + subitemDist(mapPlaceholder->second);
		++item;
		++mapPlaceholder;
		return out;
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
		mapPlaceholder = displayedItem->x.m->begin();
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
		// mapPlaceholder could already be there
		std::advance(mapPlaceholder, itemsPerPage-item);
	}
	item = 0;
	return true;
}

bool View::prevPage(){
	if (page==0){
		return false;
	}
	unsigned tPage = page-1;
	if (displayedItem->t == JType::Obj){
		firstPage();
		if (tPage != 0){
			// need to fast forward to page
			std::advance(mapPlaceholder, tPage*itemsPerPage);
		}
	} else {
		reloadPage();
	}
	page = tPage;
	return true;
}

View View::openNthItem(unsigned n){
	Jso* j = JSON::Single(JType::Null);
	std::cout << j << '\n';
	size_t index;
	unordered_map<string,Jso*>::iterator nthItem;
	switch (displayedItem->t){
		case JType::Arr:
			index = (itemsPerPage*page) + n;
			if (index < displayedItem->x.a->size()){
				j = (*displayedItem->x.a)[index];
				std::cout << "set j to " << index << " item\n";
			}
			break;
		case JType::Obj:
			if (n < item){
				nthItem = displayedItem->x.m->begin();
				std::advance(nthItem, (page*itemsPerPage)+n);
				j = nthItem->second;
				std::cout << "j has been set\n";
			}
			break;
		default:
			break;
	}
	View v;
	v.setViewItem(j);
	std::cout << j << '\n';
	v.setItemsPerPage(itemsPerPage);
	return v;
}
