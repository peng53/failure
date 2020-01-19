#ifndef JSONVIEW
#define JSONVIEW
#include "../json_adt/jso_f.h"

enum class PageState { MORE, DONE };

class View {
	private:
		Jso* displayedItem;
		unsigned itemsPerPage, page, item;
		unordered_map<string,Jso*>::iterator mapPlaceholder;

		void getItemFromArray(void (*itemPrint) (const string& s), void (*typePrint) (const string& s));
		void getItemFromObject(void (*itemPrint) (const string& s), void (*typePrint) (const string& s));

	public:
		View();
		void setViewItem(Jso* item);
		void setItemsPerPage(const unsigned count);
		void firstPage();
		bool nextPage();
		bool prevPage();
		void getItem(void (*itemPrint)(const string& s), void (*typePrint)(const string& s));
		void reloadPage();
		View openNthItem(const unsigned n);
		PageState state;
};

#endif
