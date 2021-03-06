#include "ui.h"
#include <ncurses.h>
#include <form.h>
#include <menu.h>
#include <vector>

#define return_break return

static bool yesno(WINDOW* w,const char* const desc){
	/** Prompts user for YES or NO.
	 */
	mvwprintw(w,4,2,desc);
	mvwprintw(w,5,4,"Yes / NO ?");
	wrefresh(w);
	switch (getch()){
		case 'y':
		case 'Y':
			return_break 1;
		default:
			return_break 0;
	}
	return 0;
}

static int menu_equality(WINDOW* w){
	/** Creates a menu for selecting a comparison operator, prompts
	 * for choice, frees said menu, and returns operator.
	 */
	ITEM* op[7];
	MENU* M;
	//< allocation step
	op[0] = new_item("[= ] Equal","");
	op[1] = new_item("[< ] Exclusive Before","");
	op[2] = new_item("[<=] Inclusive Before","");
	op[3] = new_item("[> ] Exclusive After","");
	op[4] = new_item("[>=] Inclusive After","");
	op[5] = new_item("[  ] None","");
	op[6] = 0; // has to be NULL?
	M = new_menu(op);
	set_menu_back(M,COLOR_PAIR(1));
	set_menu_fore(M,COLOR_PAIR(2)|A_STANDOUT);
	set_menu_win(M,w);
	set_menu_sub(M,derwin(w,6,24,13,2));
	post_menu(M);
	int c = -1;
	do { // the loop, prompts for choice
		wrefresh(w);
		switch (getch()){
			case KEY_DOWN: menu_driver(M,REQ_DOWN_ITEM); break;
			case KEY_UP: menu_driver(M,REQ_UP_ITEM); break;
			case 10: c = item_index(current_item(M));break;
		}
	} while (c==-1);
	//< cleanup step
	unpost_menu(M);
	for (size_t i=0;i<6;++i){ free_item(op[i]); }
	free_menu(M);
	wrefresh(w);
	return c;
}
static int re_eq(const char str[2]){
	/** Returns int corresponding to operator
	 */
	switch (str[0]){
		case '=': return_break 0;
		case '<': return_break (str[1]=='=') ? 2 : 1;
		case '>': return_break (str[1]=='=') ? 4 : 3;
	}
	return 5;
}
static int valid_str(const char s[100]){
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
static int getAfileName(char s[100]){
	/**
	 * Prompts user for a filename.
	 * ATM this file is not checked by ui and relies on db.
	 */
	echo();
	int r;
	do {
		mvprintw(LINES-1,0,"FILE:");
		mvgetnstr(LINES-1,5,s,99);
		r = valid_str(s);
	} while (r==1);
	noecho();
	return r;
}

static ITEM* men_ops[4];
static MENU* men_m;
static void init_main_menu(){
	men_ops[0] = new_item("(L)oad Database","Load a database for editing.");
	men_ops[1] = new_item("(N)ew Database","Create a new database for editing.");
	men_ops[2] = new_item("E(x)it Program","Exit the program.");
	men_ops[3] = 0;
	men_m = new_menu(men_ops);
	set_menu_back(men_m,COLOR_PAIR(1));
	set_menu_fore(men_m,COLOR_PAIR(2)|A_STANDOUT);
}
static void del_main_menu(){
	for (size_t i=0;i<3;++i){
		free_item(men_ops[i]);
	}
	free_menu(men_m);
}
static int get_main_menu(){
	cbreak();
	keypad(stdscr,TRUE);
	post_menu(men_m);
	int c = -1;
	do {
		switch (getch()){
			case 'L': case 'l': c = 0; break;
			case 'N': case 'n': c = 1; break;
			case 'X': case 'x': c = 2; break;
			case KEY_DOWN: menu_driver(men_m,REQ_DOWN_ITEM); break;
			case KEY_UP: menu_driver(men_m,REQ_UP_ITEM); break;
			case 10: c = item_index(current_item(men_m));break;
		}
	} while (c==-1);
	unpost_menu(men_m);
	return c;
}
namespace RecordEditor {
	FIELD* f[15];
	FORM* F;
	int rnum;
	WINDOW* wrec;
	void init(){
		/**
		 * Prepare the fields for use by:
		 * allocating them and setting their various properties
		 */
		f[0] = new_field(1,10,3,6,0,0); //uid
		f[1] = new_field(1,5,3,23,0,0); //code
		f[2] = new_field(1,2,7,3,0,0); //d1mth
		f[3] = new_field(1,2,7,6,0,0); //d1d
		f[4] = new_field(1,4,7,9,0,0); //d1yr
		f[5] = new_field(1,2,7,14,0,0); //d1hr
		f[6] = new_field(1,2,7,17,0,0); //d1min
		f[7] = new_field(1,2,11,3,0,0); //d2mth
		f[8] = new_field(1,2,11,6,0,0); //d2d
		f[9] = new_field(1,4,11,9,0,0); //d2yr
		f[10] = new_field(1,2,11,14,0,0); //d2hr
		f[11] = new_field(1,2,11,17,0,0); //d2min
		f[12] = new_field(3,26,14,2,0,0); //desc
		f[13] = new_field(1,1,18,6,0,0); //dummy
		f[14] = 0;
		for (size_t i=0;i<13;++i){ set_field_back(f[i],A_REVERSE); }
		set_field_type(f[0],TYPE_ALNUM,1);
		set_field_type(f[1],TYPE_ALNUM,1);
		set_field_type(f[2],TYPE_INTEGER,2,1,12);
		set_field_type(f[3],TYPE_INTEGER,2,1,31);
		set_field_type(f[4],TYPE_INTEGER,4,1,9999);
		set_field_type(f[5],TYPE_INTEGER,2,0,23);
		set_field_type(f[6],TYPE_INTEGER,2,0,59);
		set_field_type(f[7],TYPE_INTEGER,2,1,12);
		set_field_type(f[8],TYPE_INTEGER,2,1,31);
		set_field_type(f[9],TYPE_INTEGER,4,1,9999);
		set_field_type(f[10],TYPE_INTEGER,2,0,23);
		set_field_type(f[11],TYPE_INTEGER,2,0,59);
		field_opts_off(f[13],O_AUTOSKIP); //dummy
		F = new_form(f);
		rnum = -1;
	}
	void freem(){
		free_form(F);
		for (size_t i=0;i<14;++i){ free_field(f[i]); }
	}
	time_t get_time(const bool end){
		unsigned d[5] = {0,0,0,0,0};
		for (size_t i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+((end) ? 7 : 2)],0)); }
		return (d[0]==0 || d[1]==0) ?  time(NULL) : to_time_t(d);
	}
	void dress(){
		/**
		 * Prints the text associated with the Record creation/editor
		 * window/form.
		 */
		wattron(wrec,A_BOLD);
		mvwprintw(wrec,1,1,"Record #");
		wattroff(wrec,A_BOLD);
		if (rnum==-1){ mvwprintw(wrec,1,10,"NEW"); } //< OR UNSAVED
		else { mvwprintw(wrec,1,10,"%u",rnum); }
		mvwprintw(wrec,3,2,"UID");
		mvwprintw(wrec,3,18,"Code");
		mvwprintw(wrec,5,2,"Start Date\n   MM/DD/YYYY hh:mm");
		mvwaddch(wrec,7,5,'/');
		mvwaddch(wrec,7,8,'/');
		mvwaddch(wrec,7,16,':');
		mvwprintw(wrec,9,2,"End Date\n   MM/DD/YYYY hh:mm");
		mvwaddch(wrec,11,5,'/');
		mvwaddch(wrec,11,8,'/');
		mvwaddch(wrec,11,16,':');
		mvwprintw(wrec,13,2,"Description");
		mvwprintw(wrec,18,2,"Save?");
		box(wrec,0,0);
		wmove(wrec,3,6);
		wrefresh(wrec);
	}
	void show(const unsigned Y,const unsigned X){
		wrec = newwin(20,30,Y,X);
		wattron(wrec,COLOR_PAIR(4));
		set_form_win(F,wrec);
		set_form_sub(F,derwin(wrec,20,30,2,2));
		post_form(F);
		dress();
		wattroff(wrec,COLOR_PAIR(4));
		cbreak();
		keypad(wrec,TRUE);
	}
	void hide(){
		unpost_form(F);
		werase(wrec);
		wrefresh(wrec);
		delwin(wrec);
	}
	int driver(){
		int ch = wgetch(wrec);
		switch (ch){
			//< Arrow keys are usual for text-input. Where Up/Down function like shift-tab/tab.
			//< Backspace/Enter/Tab functions as one would except
			case 27:
			//< ESC key
			//< Followed by ESC or ENTER determines whether return is graceful or not.
				if (field_index(current_field(F))==13) return_break 1;
				switch (wgetch(wrec)){
					case 27: return_break 1;
					case 10: return_break 0;
				}
				break;
			case '\\': return_break 1;
			case KEY_LEFT: form_driver(F,REQ_LEFT_CHAR); break;
			case KEY_RIGHT: form_driver(F,REQ_RIGHT_CHAR); break;
			case KEY_BACKSPACE: form_driver(F,REQ_LEFT_CHAR); form_driver(F,REQ_DEL_CHAR); break;
			case 10: /* ENTER */
				if (field_index(current_field(F))==13) return_break 0;
			case '\t': /* TAB */ case KEY_DOWN:
				form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_END_LINE); break;
			case KEY_UP: form_driver(F, REQ_PREV_FIELD); form_driver(F, REQ_END_LINE); break;
			default: form_driver(F, ch); break;
		}
		return driver();
	}
	int edit(const unsigned Y,const unsigned X){
		/**
		 * Creates a record window at Y,X position with Record t.
		 * If t is non-null, its properties are shown to be edited.
		 * Else, user may fill the fields with their own properties.
		 * ATM does not check whether the date-fields have input.
		 * See get_start_time & get_end_time for their default values.
		 * See loop for inputs.
		 */
		show(Y,X);
		//int r;
		//do { r = driver(); } while (r==-1);
		int r = driver();
		form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_PREV_FIELD);
		hide();
		return r;
	}
	void import(const Record& t){
		/**
		 * Given a non-null Record object, populates fields.
		 * Only checks whether the Record number is not -1. --> disabled as a Record can have a -1 rnum (not in db)
		 * Which relies on null constructor.
		 */
		//if (t.rnum==-1){ return; }
		set_field_buffer(f[0],0,t.uid.c_str());
		set_field_buffer(f[1],0,t.code.c_str());
		struct tm* u = localtime(&t.ds);
		char n[6];
		snprintf(n,3,"%02u",(u->tm_mon)+1);
		set_field_buffer(f[2],0,n);
		snprintf(n,3,"%02u",u->tm_mday);
		set_field_buffer(f[3],0,n);
		snprintf(n,5,"%04u",u->tm_year+1900);
		set_field_buffer(f[4],0,n);
		snprintf(n,3,"%02u",u->tm_hour);
		set_field_buffer(f[5],0,n);
		snprintf(n,3,"%02u",u->tm_min);
		set_field_buffer(f[6],0,n);
		u = localtime(&t.de);
		snprintf(n,3,"%02u",(u->tm_mon)+1);
		set_field_buffer(f[7],0,n);
		snprintf(n,3,"%02u",u->tm_mday);
		set_field_buffer(f[8],0,n);
		snprintf(n,5,"%04u",u->tm_year+1900);
		set_field_buffer(f[9],0,n);
		snprintf(n,3,"%02u",u->tm_hour);
		set_field_buffer(f[10],0,n);
		snprintf(n,3,"%02u",u->tm_min);
		set_field_buffer(f[11],0,n);
		set_field_buffer(f[12],0,t.desc.c_str());
		rnum = t.rnum;
	}
	void reset(){
		/**
		 * Empties the fields. (spaces)
		 */
		for (size_t i=0;i<14;++i) {set_field_buffer(f[i],0," "); }
	}
	Record exportr(){
		/**
		 * Returns a new Record variable from field data.
		 */
		return Record(field_buffer(f[0],0),field_buffer(f[1],0),get_time(0),get_time(1),field_buffer(f[12],0));
	}
	Record exportr(Record &R){
		/**
		 * Assigns field values to an existing record.
		 */
		R.uid.assign(field_buffer(f[0],0),trimWS(field_buffer(f[0],0),10));
		R.code.assign(field_buffer(f[1],0),trimWS(field_buffer(f[1],0),5));
		R.desc.assign(field_buffer(f[12],0),trimWS(field_buffer(f[12],0),70));
		R.ds = get_time(0);
		R.de = get_time(1);
		return R;
	}
}
namespace FilterForm {
	FORM* F;
	FIELD* f[15];
	WINDOW* wrec;
	const char* eqs[] = {"=","<","<=",">",">="," "};
	void init(){
		f[0] = new_field(1,10,3,6,0,0); //uid
		f[1] = new_field(1,5,3,23,0,0); //code
		f[2] = new_field(1,2,5,19,0,0); // start-date eq
		f[3] = new_field(1,2,7,3,0,0); //d1mth
		f[4] = new_field(1,2,7,6,0,0); //d1d
		f[5] = new_field(1,4,7,9,0,0); //d1yr
		f[6] = new_field(1,2,7,14,0,0); //d1hr
		f[7] = new_field(1,2,7,17,0,0); //d1min
		f[8] = new_field(1,2,9,19,0,0); // end-date eq
		f[9] = new_field(1,2,11,3,0,0); //d2mth
		f[10] = new_field(1,2,11,6,0,0); //d2d
		f[11] = new_field(1,4,11,9,0,0); //d2yr
		f[12] = new_field(1,2,11,14,0,0); //d2hr
		f[13] = new_field(1,2,11,17,0,0); //d2min
		f[14] = 0;
		for (size_t i=0;i<14;++i){ set_field_back(f[i],A_REVERSE); }
		set_field_type(f[0],TYPE_ALNUM,1);
		set_field_type(f[1],TYPE_ALNUM,1);
		set_field_type(f[2],TYPE_ENUM,eqs,0,0);
		set_field_type(f[3],TYPE_INTEGER,2,1,12);
		set_field_type(f[4],TYPE_INTEGER,2,1,31);
		set_field_type(f[5],TYPE_INTEGER,4,1,9999);
		set_field_type(f[6],TYPE_INTEGER,2,0,23);
		set_field_type(f[7],TYPE_INTEGER,2,0,59);
		set_field_type(f[8],TYPE_ENUM,eqs,0,0);
		set_field_type(f[9],TYPE_INTEGER,2,1,12);
		set_field_type(f[10],TYPE_INTEGER,2,1,31);
		set_field_type(f[11],TYPE_INTEGER,4,1,9999);
		set_field_type(f[12],TYPE_INTEGER,2,0,23);
		set_field_type(f[13],TYPE_INTEGER,2,0,59);
		reset();
		F = new_form(f);
	}
	void freem(){
		for (size_t i=0;i<14;++i){ free_field(f[i]); }
		free_form(F);
	}
	void dress(){
		wattron(wrec,A_BOLD);
		mvwprintw(wrec,1,1,"Filter Records");
		wattroff(wrec,A_BOLD);
		mvwprintw(wrec,3,2,"UID");
		mvwprintw(wrec,3,18,"Code");
		mvwprintw(wrec,5,2,"Start Date  w/eq\n   MM/DD/YYYY hh:mm");
		mvwhline(wrec,5,19,' ' | A_REVERSE,2);
		mvwaddch(wrec,7,5,'/');
		mvwaddch(wrec,7,8,'/');
		mvwaddch(wrec,7,16,':');
		mvwprintw(wrec,9,2,"End Date    w/eq\n   MM/DD/YYYY hh:mm");
		mvwhline(wrec,9,19,' ' | A_REVERSE,2);
		mvwaddch(wrec,11,5,'/');
		mvwaddch(wrec,11,8,'/');
		mvwaddch(wrec,11,16,':');
		box(wrec,0,0);
		wmove(wrec,3,6);
		wrefresh(wrec);
		}
	void show(const unsigned Y,const unsigned X){
		wrec = newwin(20,30,Y,X);
		wattron(wrec,COLOR_PAIR(7));
		set_form_win(F,wrec);
		set_form_sub(F,derwin(wrec,20,30,2,2));
		post_form(F);
		dress();
		wattroff(wrec,COLOR_PAIR(7));
		cbreak();
		keypad(wrec,TRUE);
	}
	void hide(){
		unpost_form(F);
		werase(wrec);
		wrefresh(wrec);
		delwin(wrec);
	}
	int driver(){
		int ch = wgetch(wrec);
		switch (ch){
			//< Arrow keys are usual for text-input. Where Up/Down function like shift-tab/tab.
			//< Backspace/Enter/Tab functions as one would except
			case 27:
			//< ESC key
			//< Followed by ESC or ENTER determines whether return is graceful or not.
				switch (wgetch(wrec)){
					case 27: return_break 1;
					case 10: return_break 0;
				}
				break;
			case '\\': return_break 1;
			case KEY_LEFT: form_driver(F,REQ_LEFT_CHAR); break;
			case KEY_RIGHT: form_driver(F,REQ_RIGHT_CHAR); break;
			case KEY_BACKSPACE: form_driver(F,REQ_LEFT_CHAR); form_driver(F,REQ_DEL_CHAR); break;
			case 10: /* ENTER */
				switch (field_index(current_field(F))){
					case 2: case 8:
						set_field_buffer(current_field(F),0,eqs[menu_equality(wrec)]); break;
					case 13: return_break 0;
				}
			case '\t': /* TAB */ case KEY_DOWN:
				form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_END_LINE); break;
			case KEY_UP: form_driver(F, REQ_PREV_FIELD); form_driver(F, REQ_END_LINE); break;
			default: form_driver(F, ch); break;
		}
		return driver();
	}
	void reset(){
		set_field_buffer(f[0],0,"");
		set_field_buffer(f[1],0,"");
		set_field_buffer(f[2],0,"");
		set_field_buffer(f[3],0,"01");
		set_field_buffer(f[4],0,"01");
		set_field_buffer(f[5],0,"1900");
		set_field_buffer(f[6],0,"00");
		set_field_buffer(f[7],0,"00");
		set_field_buffer(f[8],0,"");
		set_field_buffer(f[9],0,"01");
		set_field_buffer(f[10],0,"01");
		set_field_buffer(f[11],0,"1900");
		set_field_buffer(f[12],0,"00");
		set_field_buffer(f[13],0,"00");
	}
	void eq_op(string &s,const int c){
		switch (c){
			case 0: s+='='; break;
			case 1: s+="<"; break;
			case 2: s+="<="; break;
			case 3: s+=">"; break;
			case 4: s+=">="; break;
		}
	}
	int prep_cus(SQLi &db){
		string e;
		bool st_eq = 0, et_eq = 0;
		if (field_buffer(f[2],0)[0]!=' '
		&& field_buffer(f[3],0)[0]!=' '
		&& field_buffer(f[4],0)[0]!=' '
		&& field_buffer(f[5],0)[0]!=' '){ // .. start-eq, needs atleast a date
			e.reserve(19);
			e+=" AND start_time";
			eq_op(e,re_eq(field_buffer(f[2],0)));
			e+="?5";
			st_eq = 1;
		}
		if (field_buffer(f[8],0)[0]!=' '
		&& field_buffer(f[9],0)[0]!=' '
		&& field_buffer(f[10],0)[0]!=' '
		&& field_buffer(f[11],0)[0]!=' '){ // .. end-eq, needs atleast a date
			e.reserve(e.length()+17);
			e+=" AND end_time";
			eq_op(e,re_eq(field_buffer(f[8],0)));
			e+="?6";
			et_eq = 1;
		}
		if (field_buffer(f[0],0)[0]!=' '){ // has UID entered
			e.reserve(e.length()+11);
			e+=" AND uid=?3";
		}
		if (field_buffer(f[1],0)[0]!=' '){ // .. code
			e.reserve(e.length()+12);
			e+=" AND code=?4";
		}
		unsigned d[10];
		for (size_t i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+3],0)); }
		for (size_t i=5;i<10;++i){ d[i] = atoi(field_buffer(f[i+4],0)); }
		db.set_cus(e,Record(field_buffer(f[0],0),field_buffer(f[1],0),to_time_t(d),to_time_t(d+5)),st_eq,et_eq);
	}
	int loop(SQLi &db){
		//int r;
		//do { r = driver(); } while (r==-1);
		int r = driver();
		form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_PREV_FIELD);
		if (r==0){ prep_cus(db); }
		return r;
	}
	int edit(const unsigned Y,const unsigned X,SQLi &db){
		show(Y,X);
		int r = loop(db);
		hide();
		return r;
	}
}
static unsigned resultsf(WINDOW* w,const size_t l,sqlite3_stmt* s,std::vector<unsigned> &ids,const unsigned y,const unsigned r){
	sqlite3_reset(s);
	sqlite3_bind_int(s,1,r);
	sqlite3_bind_int(s,2,l);
	struct tm *t;
	time_t d,d2;
	size_t n=0;
	wattron(w,COLOR_PAIR(3));
	while (sqlite3_step(s)==SQLITE_ROW && n<l){
		d = sqlite3_column_int(s,3);
		d2 = difftime(sqlite3_column_int(s,4),d);
		t = localtime(&d);
		ids[n] = sqlite3_column_int(s,0);
		mvwprintw(w,++n,1,"%05X  %-10s  %-5s  %02u/%02u/%02u %02u:%02u % -4.2f",
			ids[n],
			sqlite3_column_text(s,1),
			sqlite3_column_text(s,2),
			t->tm_mon+1,
			t->tm_mday,
			(t->tm_year)% 100,
			t->tm_hour,
			t->tm_min,
			(float)d2/3600
		);
	}
	unsigned r2 = ids[n-1];
	if (n<l){
		fill(ids.begin()+n+1,ids.end(),0);
		while (n<l){ mvwhline(w,++n,1,' ',52); }
	}
	mvwvline(w,1,6,ACS_VLINE,l);
	wattroff(w,COLOR_PAIR(3));
	wmove(w,y,0);
	wrefresh(w);
	return r2;
}

static void database_mnip(const unsigned Y,const unsigned X,const size_t l,SQLi &db){
	WINDOW* w=newwin(l,51,Y,X);
	Record rec;
	bool refresh_records = 1, refresh_yxpos = 1;
	unsigned y = 1;
	std::vector<unsigned> ids(l-2,0), pgs {0};
	size_t pg = 0;
	mvwprintw(w,0,1,"ROWID  %-10s  %-5s  MM/DD/YY hh:mm HRS","USER","CODE");
	mvwprintw(w,l-1,1,"INSERT EDIT VIEW DELETE FILTER SAVE CLOSE REFRESH");
	int r = -1;
	do {
		if (refresh_records){
			refresh_records = 0;
			if (pgs.size()==pg+1){
				pgs.push_back(resultsf(w,l-2,db.vpg,ids,y,pgs[pg]));
			} else {
				pgs[pg+1] = resultsf(w,l-2,db.vpg,ids,y,pgs[pg]);
			}
		}
		if (refresh_yxpos){
			refresh_yxpos = 0;
			wmove(w,y,0);
			wrefresh(w);
		}
		switch (getch()){
			case KEY_PPAGE:
				if (pg>0){
					refresh_records=1; --pg;
				}
				break;
			case KEY_NPAGE:
				if (ids.back()!=0){
					refresh_records=1; ++pg;
				}
				break;
			case KEY_HOME:
				if (pg!=0){
					refresh_records=1; pg=0;
				}
				break;
			case KEY_DOWN:
				if (y<l-2){
					refresh_yxpos=1;
					++y;
				}
				break;
			case KEY_UP:
				if (y>1){
					refresh_yxpos=1;
					--y;
				}
				break;
			case KEY_RIGHT:
			case 'v':
			case 'V':
				if (ids[y-1]!=0){
					RecordEditor::reset();
					RecordEditor::import(db.get_row(ids[y-1],rec));
					RecordEditor::show(0,52);
					getch();
					RecordEditor::hide();
				}
				break;
			case 'i':
				RecordEditor::reset();
				if (RecordEditor::edit(0,52)==0){
					db.chg_row(RecordEditor::exportr(rec),1);
					refresh_records=1;
				}
				break;
			case 'I':
				if (ids[y-1]==0) break;
				RecordEditor::reset();
				RecordEditor::import(db.get_row(ids[y-1],rec));
				if (RecordEditor::edit(0,52)==0){
					db.chg_row(RecordEditor::exportr(rec),1);
					refresh_records=1;
				}
				break;
			case 10: // ENTER
			case 'e':
			case 'E':
				if (ids[y-1]==0) break;
				RecordEditor::reset();
				RecordEditor::import(db.get_row(ids[y-1],rec));
				if (RecordEditor::edit(0,52)==0){
					db.chg_row(RecordEditor::exportr(rec),0);
					refresh_records=1;
				}
				break;
			case 'd':
				if (!yesno(w,"Delete row?")){
					refresh_records=1;
					break;
				}
			case 'D':
				if (ids[y-1]==0) break;
				db.del_row(ids[y-1]);
				refresh_records=1;
				break;
			case 'c':
				if (!yesno(w,"Close database?")){
					refresh_records=1;
					break;
				}
			case 'C': r=0; break;
			case 's':
				if (!yesno(w,"Save database?")){
					refresh_records=1;
					break;
				}
			case 'S': db.endbeg(); break;
			case 'r': refresh_records=1;pg=0; break;
			case 'f':
				if (FilterForm::edit(0,52,db)==0){
					refresh_yxpos=1;
					refresh_records=1;
					pg=0;
				}
				break;
			case 'F':
				db.set_vpg();
				refresh_yxpos=1;
				refresh_records=1;
				pg=0;
				break;
		}
	} while (r==-1);
	delwin(w);
}
int init_viewr(){
	initscr();
	if (LINES<20 || COLS<30){
		printw("Your terminal window needs to be at least 20x30! Sorry.");
		getch();
		endwin();
		return 0;
	}
	noecho();
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK); //
	init_pair(2,COLOR_MAGENTA,COLOR_BLACK); // MENU disable
	init_pair(3,COLOR_CYAN,COLOR_BLACK); // results row
	init_pair(4,COLOR_YELLOW,COLOR_BLACK); // editor
	init_pair(5,COLOR_GREEN,COLOR_BLACK); //
	init_pair(6,COLOR_BLUE,COLOR_BLACK); //
	init_pair(7,COLOR_MAGENTA,COLOR_BLACK); // filter
	init_main_menu();
	RecordEditor::init();
	FilterForm::init();
	return 1;
}
void nloop(){
	char s[100];
	int l;
	sqlite3* db;
	do {
		l = get_main_menu();
		if (l!=2){
			//< Where l==0 is existing, 1 is new.
			if (getAfileName(s)==0 && open_sqdb(&db,s,l)==0){
				SQLi mydb(db);
				database_mnip(0,0,LINES-1,mydb);
				erase();
			} else {
				printw("Error: File ");
				printw((l==0) ? "doesn't exists / fit schema?" : "already exists / cannot be created?");
			}
		}
	} while (l!=2);
}
void cleanup(){
	FilterForm::freem();
	RecordEditor::freem();
	del_main_menu();
	endwin();
}
