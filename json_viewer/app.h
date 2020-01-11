#ifndef JSONAPP
#define JSONAPP
#include "view.h"
#include <stack>
#include <unordered_map>

enum class Command {NEXTPG, PREVPG, CLOSEVIEW, OPENVIEW, QUIT, MOVEUP, MOVEDOWN, RELOAD};

class App {
	private:
		unsigned width, height;
		std::stack<View> views;
		std::unordered_map<int, Command> keybinds;
		unsigned selectedLine;
		bool _running;

	public:
		App();
		void draw();
		void setRootViewItem(View& item);
		void setDimensions(const unsigned _width, const unsigned _height);
		void nextPage();
		void prevPage();
		void selectUpperItem();
		void selectLowerItem();
		void openSelectedItem();
		void closeView();
		void reload();
		void quit();
		void keySym(const int c);
		void addBind(const int key, const Command cmd);
		bool running();

};

#endif
