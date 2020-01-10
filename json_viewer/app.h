#ifndef JSONAPP
#define JSONAPP
#include "view.h"
#include <ncurses.h>
#include <stack>
#include <unordered_map>

enum class Command {NEXTPG, PREVPG, CLOSEVIEW, OPENVIEW, QUIT, MOVEUP, MOVEDOWN};

class App {
	private:
		View rootViewItem;
		unsigned width, height;
		std::stack<View> views;
		std::unordered_map<int, Command> keybinds;
		unsigned selectedLine;
		
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
		void quit();
		void keySym(int c);
		void addBind(int key, Command cmd);
		bool running;

};

#endif
