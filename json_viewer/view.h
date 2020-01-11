#ifndef JSONVIEW
#define JSONVIEW
#include <memory>
#include "../json_adt/jso_f.h"

enum class PageState { MORE, DONE };

class View {
	private:
		Jso* displayedItem;
		unsigned itemsPerPage, page, item;
		unordered_map<string,Jso*>::iterator mapPlaceholder;

		void getItemFromArray(void (*f) (const string& s));
		void getItemFromObject(void (*f) (const string& s));

	public:
		View();
		void setViewItem(Jso* item);
		void setItemsPerPage(unsigned count);
		void firstPage();
		bool nextPage();
		bool prevPage();
		void getItem(void (*f)(const string& s));
		void reloadPage();
		View openNthItem(unsigned n);
		PageState state;
};

#endif
