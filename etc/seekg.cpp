/* seekg.cpp
 * 'Editor' with ncurses 'cli' which attempts to write to a file
 * inplace? Kinda like an overwrite editor. Writes to 'test.txt'
 * an 80x24 file populated by '.' Typing seems to write changes
 * right away and enter ends the program. Doesn't seem to be able
 * to edit existing files.
 *
 */
#include <ncurses.h>
#include <fstream>

using namespace std;

int main(){
	initscr();
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	ofstream myfile("test.txt",fstream::binary | fstream::in | fstream::out);
	//ofstream myfile("test.txt",fstream::in | fstream::out);
	for (unsigned l=0;l<24;++l){
		for (unsigned c=0;c<80;++c){
			myfile<<'.';
		}
		myfile<<'\n';
	}
	int r = 1;
	int ch;
	bool changepos=1;
	move(0,0);
	int y = 0, x = 0;
	while (r==1){
		if (changepos){
			changepos=0;
			move(y,x);
			refresh();
			myfile.seekp(y*81+x);
		}
		ch = getch();
		switch (ch){
			case KEY_DOWN:
				if (y<23){
					++y;
					changepos=1;
				}
				break;
			case KEY_UP:
				if (y>0){
					--y;
					changepos=1;
				}
				break;
			case KEY_LEFT:
				if (x>0){
					--x;
					changepos=1;
				}
				break;
			case KEY_RIGHT:
				if (x<79){
					++x;
					changepos=1;
				}
				break;
			case KEY_BACKSPACE:
				if (x>0){
					addch(' ');
					myfile.seekp(-1,ios_base::cur);
					myfile<<' ';
					move(y,--x);
					refresh();
				}
				break;
			case 10:
				r = 0;
				break;
			default:
				if (ch>=32 && ch<=126){
					addch(ch);
					myfile<<(char)ch;
					changepos=1;
					if (x!=79){
						++x;
					}
				}
				break;
		}
	}
	endwin();
	//myfile.seekp(2,ios_base::beg);
	//cout<<myfile.tellp();
	//for (unsigned i=0;i<10;++i){
	//	for (unsigned n=0;n<81;++n){
	//		myfile<<'*';
	//	}
	//	myfile<<'\n';
	//}
	/*
	myfile.seekp(3*82+3);
	cout<<myfile.tellp();
	myfile << '1';
	cout<<"WHA\n";
	*/
	//myfile <<

	myfile.close();
	return 0;
}
