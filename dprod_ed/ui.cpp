#include "ui.h"
#include <cstdlib>
#include <cstdio>

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
void populate_fields(FIELD *field[15],Record &t){
	if (t.rnum==-1){ return; }
	set_field_buffer(field[0],0,t.uid.c_str());
	set_field_buffer(field[1],0,t.code.c_str());
	struct tm* u = gmtime(&t.ds);
	char n[6];
	snprintf(n,3,"%02d",(u->tm_mon)+1);
	set_field_buffer(field[2],0,n);
	snprintf(n,3,"%02d",u->tm_mday);
	set_field_buffer(field[3],0,n);
	snprintf(n,5,"%04d",u->tm_year+1900);
	set_field_buffer(field[4],0,n);
	snprintf(n,3,"%02d",u->tm_hour);
	set_field_buffer(field[5],0,n);
	snprintf(n,3,"%02d",u->tm_min);
	set_field_buffer(field[6],0,n);
	u = gmtime(&t.de);
	snprintf(n,3,"%02d",(u->tm_mon)+1);
	set_field_buffer(field[7],0,n);
	snprintf(n,3,"%02d",u->tm_mday);
	set_field_buffer(field[8],0,n);
	snprintf(n,5,"%04d",u->tm_year+1900);
	set_field_buffer(field[9],0,n);
	snprintf(n,3,"%02d",u->tm_hour);
	set_field_buffer(field[10],0,n);
	snprintf(n,3,"%02d",u->tm_min);
	set_field_buffer(field[11],0,n);
	set_field_buffer(field[12],0,t.desc.c_str());
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
int prompt_rnum(){
	char s[10] = "~";
	int r = 1;
	while (r==1){
		mvprintw(LINES-1,0,"RNUM:");
		mvgetnstr(LINES-1,5,s,10);
		if (s[0]!='~') return atoi(s);
		if (s[0]!='\\') return -2;
	}
	return -1;
}
int make_rec_win(unsigned Y,unsigned X,Record &t){
	int ch;
	int r = -1;
	FIELD *field[15];
	prepare_fields(field);
	if (t.rnum!=-1) populate_fields(field,t);
	FORM *frec = new_form(field);
	WINDOW *wrec = newwin(20,30,Y,X);
	keypad(wrec,TRUE);
	set_form_win(frec,wrec);
	set_form_sub(frec,derwin(wrec,20,30,2,2));
	post_form(frec);
	dress_rec_win(wrec,t.rnum);
    while(r==-1){
		ch = wgetch(wrec);
		switch(ch) {
			case 27: // ESC key
				switch (wgetch(wrec)){
					case 27: r = 1; break;
					case 10: r = 0; break;
				}
				break;
			case '\\': r = 1; break;
			case KEY_LEFT: form_driver(frec,REQ_LEFT_CHAR); break;
			case KEY_RIGHT: form_driver(frec,REQ_RIGHT_CHAR); break;
			case KEY_BACKSPACE: form_driver(frec,REQ_LEFT_CHAR); form_driver(frec,REQ_DEL_CHAR); break;
			case '\t':
			case 10: // ENTER KEY
			case KEY_DOWN: form_driver(frec, REQ_NEXT_FIELD); form_driver(frec, REQ_END_LINE); break;
			case KEY_UP: form_driver(frec, REQ_PREV_FIELD); form_driver(frec, REQ_END_LINE); break;
			default: form_driver(frec, ch); break;
		}
	}
	switch (r){
		case 0: // user exits gracefully
			form_driver(frec, REQ_NEXT_FIELD); form_driver(frec, REQ_PREV_FIELD);
			t = Record(field_buffer(field[0],0),field_buffer(field[1],0),get_start_time(field),get_end_time(field),field_buffer(field[12],0));
			break;
		case 1: // user wants escape
			break;
	}
	unpost_form(frec);
	clean_up_rec_form(frec,field);
	delwin(wrec);
	return r;
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
