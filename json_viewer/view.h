#ifndef JSONVIEW
#define JSONVIEW
#include <memory>
#include "../json_adt/jso_f.h"

using std::unique_ptr;

class View {
	private:
		Jso* displayedItem;
		unsigned x, y, width, height;

	public:
		View();
		~View();
		void setViewItem(Jso* item);
		void setOrigin(unsigned x, unsigned y);
		void setDimensions(unsigned width, unsigned height);
		bool draw();
		bool nextPage();
		bool prevPage();
		bool selectUpperItem();
		bool selectLowerItem();
		unique_ptr<View> openSelectedItem();
		void close();
};

#endif
