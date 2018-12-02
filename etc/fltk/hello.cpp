#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <Fl/Fl_Browser.H>
#include <cstdio>

using namespace std;

namespace Gui {
	Fl_Window* win;
	Fl_Button* but;
	Fl_Input* inp;
	Fl_Browser* ml_o;
	void quit_call(Fl_Widget* p,void *){
		inp->value("test!");
		ml_o->add("testss");
		//win->hide();
	}
	void init(){
		win = new Fl_Window(0,0,"Testing" );
		win->size_range(100,100);
		win->begin();
		but = new Fl_Button(20,20,100,25,"Button 1");
		inp = new Fl_Input(20,50,100,25);
		ml_o= new Fl_Browser(20,70,100,75);
		but->callback(quit_call);
		win->end();
	}
	int run(){
		win->show();
		return Fl::run();
	}
	void clean(){
		printf("deleted fltk window");
		delete inp;
		delete but;
		delete win;
	}
};


int main(){
	Gui::init();
	Gui::run();
	Gui::clean();
	return 0;

}
