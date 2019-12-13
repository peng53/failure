#ifndef JSONAPP
#define JSONAPP
#include "view.h"

class App {
	private:

	public:
		App();
		~App();
		void draw();
		void setRootViewItem(Jso* item);
		void nextPage();
		void prevPage();
		void selectUpperItem();
		void selectLowerItem();
		void openSelectedItem();
		void closeView();
		void keySym(int c);
		void addBind(int c, string& cmd);

		bool running;
};

#endif
