#include "view.h"
#include <string>
#include <iostream>
#include <iterator>

using std::to_string;
using std::string;

template <class ForwardIter>
static ForwardIter iteratorToNthPage(ForwardIter it, const size_t page, const size_t itemsPerPage){
	std::advance(it, page*itemsPerPage);
	return it;
}

View::View(): page(0), item(0){

}

void View::setViewItem(Jso* viewItem){
	displayedItem = viewItem;
	firstPage();
}

void View::setItemsPerPage(const unsigned count){
	itemsPerPage = count;
	firstPage();
}

static const string JsoStringRep(Jso *j){
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

static const string subitemDist(const Jso* j){
	switch (j->t){
		case JType::Arr:
			return "~[]";
		case JType::Obj:
			return "~{}";
		case JType::Str:
			return "(s)";
		case JType::Num:
			return "(f)";
		case JType::True:
			return "(1)";
		case JType::False:
			return "(0)";
		case JType::Null:
			return "(n)";
		default:
			return "";
	}
}

void View::getItem(void (*itemPrint) (const string& s), void (*typePrint) (const string& s)){
	if (item >= itemsPerPage){
		state = PageState::DONE;
	}
	if (state != PageState::MORE){
		return;
	}
	switch (displayedItem->t){
		case JType::Arr:
			getItemFromArray(itemPrint, typePrint);
			break;
		case JType::Obj:
			getItemFromObject(itemPrint, typePrint);
			break;
		default:
			typePrint(subitemDist(displayedItem));
			itemPrint(JsoStringRep(displayedItem));
			state = PageState::DONE;
			break;
	}
}

void View::getItemFromArray(void (*itemPrint) (const string& s), void (*typePrint) (const string& s)){
	size_t index = item + itemsPerPage*page;
	if (index >= displayedItem->x.a->size()){
		state = PageState::DONE;
	} else {
		++item;
		typePrint(subitemDist((*displayedItem->x.a)[index]));
		itemPrint(JsoStringRep((*displayedItem->x.a)[index]));
	}
}

void View::getItemFromObject(void (*itemPrint) (const string& s), void (*typePrint) (const string& s)){
	size_t index = item + itemsPerPage*page;
	if (index >= displayedItem->x.m->size()){
		state = PageState::DONE;
	} else {
		typePrint(subitemDist(mapPlaceholder->second));
		itemPrint(mapPlaceholder->first);
		++item;
		++mapPlaceholder;
	}
}

void View::reloadPage(){
	state = PageState::MORE;
	item = 0;
	if (displayedItem->t == JType::Obj){
		mapPlaceholder = iteratorToNthPage(
			displayedItem->x.m->begin(),
			page,
			itemsPerPage
		);
	}
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


View View::openNthItem(const unsigned n){
	Jso* j = JSON::Single(JType::Null);
	size_t index;
	unordered_map<string,Jso*>::iterator nthItem;
	switch (displayedItem->t){
		case JType::Arr:
			index = (itemsPerPage*page) + n;
			if (index < displayedItem->x.a->size()){
				j = (*displayedItem->x.a)[index];
			}
			break;
		case JType::Obj:
			if (n < item){
				nthItem = iteratorToNthPage(
					displayedItem->x.m->begin(),
					page,
					itemsPerPage
				);
				std::advance(nthItem, n);
				j = nthItem->second;
			}
			break;
		default:
			break;
	}
	View v;
	v.setViewItem(j);
	v.setItemsPerPage(itemsPerPage);
	return v;
}
