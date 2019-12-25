#ifndef JSONVIEW
#define JSONVIEW
#include <memory>
#include "../json_adt/jso_f.h"

using std::unique_ptr;

enum class PageState { MORE, DONE };

class View {
	private:
		Jso* displayedItem;
		unsigned itemsPerPage, page, item;
	public:
		View();
		~View();
		void setViewItem(Jso* item);
		void setItemsPerPage(unsigned count);
		void firstPage();
		void lastPage();
		bool nextPage();
		bool prevPage();
		void getItem(void (*f)(string& s));
		void reloadPage();
		unique_ptr<View> openNthItem();
		PageState state;
};

#endif
