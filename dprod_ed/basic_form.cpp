#include <form.h>
#include <ncurses.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

time_t to_time_t(unsigned *d){
	time_t t = time(NULL);
	struct tm* u = gmtime(&t);
	u->tm_mon = d[0]-1;
	u->tm_mday = d[1];
	u->tm_year = d[2] - 1900;
	u->tm_hour = d[3];
	u->tm_min = d[4];
	u->tm_sec = 0;
	return mktime(u);
}
size_t trimWS(char *s,size_t m){
	for (size_t i=m-1;i>=0;--i){
		if (s[i]!=' ') return ++i;
	}
	return m;
}
time_t field_time_t(FIELD* f){
	unsigned d[5] = {0,0,0,0,0};
	for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f+i,0)); }
	return (d[0]==0 || d[1]==0) ? to_time_t(d) : time(NULL);
}
struct Record {
	string uid;
	string code;
	string desc;
	time_t ds;
	time_t de;
};
ostream& operator<<(ostream& OUT,Record& R){
	OUT << R.uid << '\n' << R.code << '\n';
	struct tm* d = gmtime(&R.ds);
	OUT<<setfill('0')<<setw(2)<<d->tm_mon+1<<'/'<<setw(2)<<d->tm_mday<<'/'<<setw(2)<<(d->tm_year)%100<<" - "<<setw(2)<<d->tm_hour<<':'<<setw(2)<<d->tm_min<<'\n';
	d = gmtime(&R.de);
	OUT<<setfill('0')<<setw(2)<<d->tm_mon+1<<'/'<<setw(2)<<d->tm_mday<<'/'<<setw(2)<<(d->tm_year)%100<<" - "<<setw(2)<<d->tm_hour<<':'<<setw(2)<<d->tm_min<<'\n';
	OUT << R.desc << '\n';
	return OUT;
}

void prepare_fields(FIELD *field[15]){
	field[0] = new_field(1,10,3,6,0,0); //uid
	field[1] = new_field(1,5,3,23,0,0); //code
	field[2] = new_field(1,2,7,3,0,0); //d1mth
	field[3] = new_field(1,2,7,6,0,0); //d1d
	field[4] = new_field(1,4,7,9,0,0); //d1yr
	field[5] = new_field(1,2,7,14,0,0); //d1hr
	field[6] = new_field(1,2,7,17,0,0); //d1min
	field[7] = new_field(1,2,11,3,0,0); //d2mth
	field[8] = new_field(1,2,11,6,0,0); //d2d
	field[9] = new_field(1,4,11,9,0,0); //d2yr
	field[10] = new_field(1,2,11,14,0,0); //d2hr
	field[11] = new_field(1,2,11,17,0,0); //d2min
	field[12] = new_field(3,26,14,2,0,0); //desc
	field[13] = new_field(1,1,1,1,0,0); //dummy
	field[14] = 0;
	for (unsigned i=0;i<13;++i){ set_field_back(field[i],A_REVERSE); }
	set_field_type(field[0],TYPE_ALNUM,1);
	set_field_type(field[1],TYPE_ALNUM,1);
	set_field_type(field[2],TYPE_INTEGER,2,1,12);
	set_field_type(field[3],TYPE_INTEGER,2,1,31);
	set_field_type(field[4],TYPE_INTEGER,4,1,9999);
	set_field_type(field[5],TYPE_INTEGER,2,0,23);
	set_field_type(field[6],TYPE_INTEGER,2,0,59);
	set_field_type(field[7],TYPE_INTEGER,2,1,12);
	set_field_type(field[8],TYPE_INTEGER,2,1,31);
	set_field_type(field[9],TYPE_INTEGER,4,1,9999);
	set_field_type(field[10],TYPE_INTEGER,2,0,23);
	set_field_type(field[11],TYPE_INTEGER,2,0,59);
	field_opts_off(field[13],O_AUTOSKIP); //dummy
}
void clean_up_rec_form(FORM *form,FIELD *field[15]){
	free_form(form);
	for (unsigned i=0;i<14;++i){ free_field(field[i]); }
}
void dress_rec_win(WINDOW* W,int rnum){
	wattron(W,A_BOLD);
	mvwprintw(W,1,1,"Record #");
	wattroff(W,A_BOLD);
	if (rnum==-1){ mvwprintw(W,1,10,"NEW"); }
	else { mvwprintw(W,1,10,"%d",rnum); }
	mvwprintw(W,3,2,"UID");
	mvwprintw(W,3,18,"Code");
	mvwprintw(W,5,2,"Start Date\n   MM/DD/YYYY hh:mm");
	mvwaddch(W,7,5,'/');
	mvwaddch(W,7,8,'/');
	mvwaddch(W,7,16,':');
	mvwprintw(W,9,2,"End Date\n   MM/DD/YYYY hh:mm");
	mvwaddch(W,11,5,'/');
	mvwaddch(W,11,8,'/');
	mvwaddch(W,11,16,':');
	mvwprintw(W,13,2,"Description");
	box(W,0,0);
	wmove(W,3,6);
	wrefresh(W);
}

int make_rec_win(Record &t){
	int ch;
	FIELD *field[15];
	prepare_fields(field);
	FORM *frec = new_form(field);
	WINDOW *wrec = newwin(20,30,0,0);
	keypad(wrec,TRUE);
	set_form_win(frec,wrec);
	set_form_sub(frec,derwin(wrec,20,30,2,2));
	post_form(frec);
	dress_rec_win(wrec,-1);
    while((ch = wgetch(wrec)) != KEY_F(1)){
		switch(ch) {
			case 27: // ESC key
				switch (wgetch(wrec)){
					case 27:
						unpost_form(frec);
						clean_up_rec_form(frec,field);
						delwin(wrec);
						return 1;
						break;
					case 10:
						unpost_form(frec);
						clean_up_rec_form(frec,field);
						delwin(wrec);
						return 0;
						break;
				}
				break;
			case '\\':
				unpost_form(frec);
				clean_up_rec_form(frec,field);
				delwin(wrec);
				return 1;
				break;
			case KEY_LEFT:
				form_driver(frec,REQ_LEFT_CHAR);
				break;
			case KEY_RIGHT:
				form_driver(frec,REQ_RIGHT_CHAR);
				break;
			case KEY_BACKSPACE:
				form_driver(frec,REQ_LEFT_CHAR);
				form_driver(frec,REQ_DEL_CHAR);
				break;
			case '\t':
			case 10: // ENTER KEY
			case KEY_DOWN:
				form_driver(frec, REQ_NEXT_FIELD);
				form_driver(frec, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(frec, REQ_PREV_FIELD);
				form_driver(frec, REQ_END_LINE);
				break;
			default:
				form_driver(frec, ch);
				break;
		}
	}
	form_driver(frec, REQ_NEXT_FIELD);
	form_driver(frec, REQ_PREV_FIELD);
	t.uid = string(field_buffer(field[0],0));
	cout << t.uid << '\n';
	getch();
	t.code = string(field_buffer(field[1],0));
	t.desc = string(field_buffer(field[12],0));
	//t.ds = field_time_t(field[2]);
	//t.de = field_time_t(field[7]);
	unpost_form(frec);
	clean_up_rec_form(frec,field);
	delwin(wrec);
}


int main(){
	Record t;
	initscr();
	start_color();
	cbreak();
	noecho();
	make_rec_win(t);
	endwin();
	cout << '\"'<< t.uid << '\"'<< '\n';
	cout << '\"'<< t.code << '\"'<< '\n';
	cout << '\"'<< t.desc << '\"'<< '\n';
	//cout << t << '\n';
	return 0;
}