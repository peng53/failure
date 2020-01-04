#ifndef JSONAPP
#define JSONAPP
#include "view.h"
#include <ncurses.h>

class App {
	private:
		View rootViewItem;
		unsigned width, height;
		WINDOW* viewwin;
		
	public:
		App();
		~App();
		void draw();
		void setRootViewItem(View& item);
		void setDimensions(unsigned _width, unsigned _height);
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
