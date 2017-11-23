#ifndef D_UI
#define D_UI
#include "db.cpp"
#include "ui.h"
#include <cstdlib>
#include <ncurses.h>
#include <form.h>
#include <algorithm>

//~ time_t field_time_t(FIELD* f){
	//~ unsigned d[5] = {0,0,0,0,0};
	//~ for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f+i,0)); }
	//~ return (d[0]==0 || d[1]==0) ? to_time_t(d) : time(NULL);
//~ }
time_t get_start_time(FIELD* f[15]){
	unsigned d[5] = {0,0,0,0,0};
	for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+2],0)); }
	return (d[0]==0 || d[1]==0) ? to_time_t(d) : time(NULL);
}
time_t get_end_time(FIELD* f[15]){
	unsigned d[5] = {0,0,0,0,0};
	for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+7],0)); }
	return (d[0]==0 || d[1]==0) ? to_time_t(d) : time(NULL);
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
						return 1;
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
	//~ t.uid = string(field_buffer(field[0],0),trimWS(field_buffer(field[0],0),10));
	//~ t.code = string(field_buffer(field[1],0),trimWS(field_buffer(field[1],0),5));
	//~ t.desc = string(field_buffer(field[12],0),trimWS(field_buffer(field[12],0),70));
	//~ t.ds = get_start_time(field);
	//~ t.de = get_end_time(field);
	//t = Record(field_buffer(field[0],0),10,field_buffer(field[1],0),5,get_start_time(field),get_end_time(field),field_buffer(field[12],0),70);
	t = Record(field_buffer(field[0],0),field_buffer(field[1],0),get_start_time(field),get_end_time(field),field_buffer(field[12],0));
	unpost_form(frec);
	clean_up_rec_form(frec,field);
	delwin(wrec);
	return 0;
}

int valid_str(char *s){
	/**
	Checks if string has length and counts length.
	Return codes:
	0: success
	1: no/bad input
	2: special input
	*/
	if (s[0]=='\0' || s[0]=='\n' && s[1]=='\0'){ return 1; }
	if (s[0]=='\\'){ return 2; }
	return 0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t M){
	int r = 1;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		r = valid_str(s);
	}
	return r==0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char* s,size_t M,unsigned &n){
	int r = 1;
	char *end;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		if (s[0]=='\\'){ return 0; }
		n = (unsigned)strtoul(s,&end,10);
		if (!*end){ return 1; }
	}
	return r==0;
}
void record_creation_win(WINDOW* W){
	/**
	Prints parts of the record creation window.
	*/
	wattron(W,A_BOLD);
	mvwprintw(W,1,2,"Record Creation");
	wattroff(W,A_BOLD);
	/// the fields
	wattron(W,COLOR_PAIR(1));
	mvwhline(W,3,6,ACS_BLOCK,10);
	mvwhline(W,3,23,ACS_BLOCK,5);
	mvwhline(W,7,3,ACS_BLOCK,16);
	mvwhline(W,11,3,ACS_BLOCK,16);
	for (unsigned l=LINES-2;l>13;--l){
		mvwhline(W,l,3,ACS_BLOCK,25);
	}
	wattroff(W,COLOR_PAIR(1));
	/// labels above the fields
	mvwprintw(W,3,2,"UID");
	mvwprintw(W,3,18,"Code");
	mvwprintw(W,5,2,"Start Date\n   MM/DD/YYYY hh:mm");
	mvwprintw(W,9,2,"End Date\n   MM/DD/YYYY hh:mm");
	mvwprintw(W,13,2,"Description");
	mvwaddch(W,7,5,'/');
	mvwaddch(W,7,8,'/');
	mvwaddch(W,7,13,' ');
	mvwaddch(W,7,16,':');
	mvwaddch(W,11,5,'/');
	mvwaddch(W,11,8,'/');
	mvwaddch(W,11,13,' ');
	mvwaddch(W,11,16,':');
	box(W,0,0);
	wrefresh(W);
}
int build_a_record(unsigned short Y,unsigned short X,Record &T){
	WINDOW* a_record = newwin(LINES-1,31,Y,X);
	record_creation_win(a_record);
	wattron(a_record,A_REVERSE);
	char desc[26] = "\0";
	unsigned date[10];
	if (!valid_wrap(a_record,3,6,&T.uid[0],10) ||
		!valid_wrap(a_record,3,23,&T.code[0],5) ||
		!valid_wrap(a_record,7,3,desc,2,date[0]) ||
		!valid_wrap(a_record,7,6,desc,2,date[1]) ||
		!valid_wrap(a_record,7,9,desc,4,date[2]) ||
		!valid_wrap(a_record,7,14,desc,2,date[3]) ||
		!valid_wrap(a_record,7,17,desc,2,date[4]) ||
		!valid_wrap(a_record,11,3,desc,2,date[5]) ||
		!valid_wrap(a_record,11,6,desc,2,date[6]) ||
		!valid_wrap(a_record,11,9,desc,4,date[7]) ||
		!valid_wrap(a_record,11,14,desc,2,date[8]) ||
		!valid_wrap(a_record,11,17,desc,2,date[9])
	){ return 1; }
	T.ds = to_time_t(date);
	T.de = to_time_t(date+5);
	mvwgetnstr(a_record,14,3,desc,25); ///< desc-l1
	for (unsigned l = 15;valid_str(desc)==0;++l){
		T.append_desc(desc);
		desc[0]='\0';
		mvwgetnstr(a_record,l,3,desc,25);
	}
	wattroff(a_record,A_REVERSE);
	wclear(a_record);
	delwin(a_record);
	return 0;
}
int getAfileName(char *s){
	int r = 1;
	while (r==1){
		mvprintw(LINES-1,0,"FILE:");
		mvgetnstr(LINES-1,5,s,99);
		r = valid_str(s);
		if (r==2){ return 2; }
	}
	return 0;
}
int show_results(sqlite3_stmt* s){
	struct tm *t;
	time_t d,d2;
	unsigned ml = std::min(25,LINES-1);
	for (unsigned n = 0; sqlite3_step(s)==100 && n<ml; ++n){
		mvprintw(n,0,"%02d %-10s %-5s",n,sqlite3_column_text(s,0),sqlite3_column_text(s,3));
		d = sqlite3_column_int(s,1);
		d2 = difftime(sqlite3_column_int(s,2),d);
		t = gmtime(&d);
		mvprintw(n,21,"%02d/%02d/%02d %02d:%02d %1.2fhrs",t->tm_mon+1,t->tm_mday,(t->tm_year)% 100,t->tm_hour,t->tm_min,(float)d2/3600);
	}
	return 0;
}

#endif