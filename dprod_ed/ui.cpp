#include "ui.h"
#include <cstdlib>
#include <cstdio>

void mvprintw(unsigned Y,unsigned X,const Record &t){
	struct tm* d = gmtime(&t.ds);
	mvprintw(Y,X,"\"%s\"",t.uid.c_str());
	mvprintw(Y+1,X,"\"%s\"",t.code.c_str());
	mvprintw(Y+2,X,"%02d/%02d/%02d : %02d:%02d",d->tm_mon+1,d->tm_mday,(d->tm_year)%100,d->tm_hour,d->tm_min);
	d = gmtime(&t.de);
	mvprintw(Y+3,X,"%02d/%02d/%02d : %02d:%02d",d->tm_mon+1,d->tm_mday,(d->tm_year)%100,d->tm_hour,d->tm_min);
	mvprintw(Y+4,X,"\"%s\"",t.desc.c_str());
}

nRecord::nRecord(): rnum(-1){
	/**
	 * Prepare the fields for use by:
	 * allocating them and setting their various properties
	 */
	//rnum = -1;
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
	f[13] = new_field(1,1,1,1,0,0); //dummy
	f[14] = 0;
	for (unsigned i=0;i<13;++i){ set_field_back(f[i],A_REVERSE); }
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
}
nRecord::~nRecord(){
	free_form(F);
	for (unsigned i=0;i<14;++i){ free_field(f[i]); }
}

void nRecord::dress_rec_win(WINDOW* W){
	/**
	 * Prints the text associated with the Record creation/editor
	 * window/form.
	 */
	wattron(W,A_BOLD);
	mvwprintw(W,1,1,"Record #");
	wattroff(W,A_BOLD);
	if (rnum==-1){ mvwprintw(W,1,10,"NEW"); } //< OR UNSAVED
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
time_t nRecord::get_start_time(){
	unsigned d[5] = {0,0,0,0,0};
	for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+2],0)); }
	return (d[0]==0 || d[1]==0) ? time(NULL) : to_time_t(d);
}
time_t nRecord::get_end_time(){
	unsigned d[5] = {0,0,0,0,0};
	for (unsigned i=0;i<5;++i){ d[i] = atoi(field_buffer(f[i+7],0)); }
	return (d[0]==0 || d[1]==0) ?  time(NULL) : to_time_t(d);
}
WINDOW* nRecord::view(unsigned Y,unsigned X){
	WINDOW *wrec = newwin(20,30,Y,X);
	set_form_win(F,wrec);
	set_form_sub(F,derwin(wrec,20,30,2,2));
	post_form(F);
	dress_rec_win(wrec);
	noecho();
	cbreak();
	keypad(wrec,TRUE);
	return wrec;
}
void nRecord::un_view(WINDOW *wrec){
	unpost_form(F);
	delwin(wrec);
	echo();
}
int nRecord::driver(WINDOW *wrec){
	int ch = wgetch(wrec);
	switch(ch) {
		//< Arrow keys are usual for text-input. Where Up/Down function like shift-tab/tab.
		//< Backspace/Enter/Tab functions as one would except
		case 27:
		//< ESC key
		//< Followed by ESC or ENTER determines whether return is graceful or not.
			switch (wgetch(wrec)){
				case 27: return 1; break;
				case 10: return 0; break;
			}
			break;
		case '\\': return 1; break;
		case KEY_LEFT: form_driver(F,REQ_LEFT_CHAR); break;
		case KEY_RIGHT: form_driver(F,REQ_RIGHT_CHAR); break;
		case KEY_BACKSPACE: form_driver(F,REQ_LEFT_CHAR); form_driver(F,REQ_DEL_CHAR); break;
		case '\t': /* TAB */ case 10: /* ENTER */ case KEY_DOWN:
			form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_END_LINE); break;
		case KEY_UP: form_driver(F, REQ_PREV_FIELD); form_driver(F, REQ_END_LINE); break;
		default: form_driver(F, ch); break;
	}
	return -1;
}

int nRecord::edit(unsigned Y,unsigned X){
	/**
	 * Creates a record window at Y,X position with Record t.
	 * If t is non-null, its properties are shown to be edited.
	 * Else, user may fill the fields with their own properties.
	 * ATM does not check whether the date-fields have input.
	 * See get_start_time & get_end_time for their default values.
	 * See loop for inputs.
	 */
	WINDOW *wrec = view(Y,X);
	int r = -1;
	while (r==-1) r = driver(wrec);
	form_driver(F, REQ_NEXT_FIELD); form_driver(F, REQ_PREV_FIELD);
	un_view(wrec);
	return r;
}
Record nRecord::exportr(){
	/**
	 * Returns a new Record variable from field data.
	 */
	return Record(field_buffer(f[0],0),field_buffer(f[1],0),get_start_time(),get_end_time(),field_buffer(f[12],0));
}
void nRecord::depopulate(){
	/**
	 * Empties the fields. (spaces)
	 */
	for (unsigned i=0;i<14;++i) {set_field_buffer(f[i],0," "); }
}
void nRecord::populate(const Record &t){
	/**
	 * Given a non-null Record object, populates fields.
	 * Only checks whether the Record number is not -1. --> disabled as a Record can have a -1 rnum (not in db)
	 * Which relies on null constructor.
	 */
	//if (t.rnum==-1){ return; }
	set_field_buffer(f[0],0,t.uid.c_str());
	set_field_buffer(f[1],0,t.code.c_str());
	struct tm* u = gmtime(&t.ds);
	char n[6];
	snprintf(n,3,"%02d",(u->tm_mon)+1);
	set_field_buffer(f[2],0,n);
	snprintf(n,3,"%02d",u->tm_mday);
	set_field_buffer(f[3],0,n);
	snprintf(n,5,"%04d",u->tm_year+1900);
	set_field_buffer(f[4],0,n);
	snprintf(n,3,"%02d",u->tm_hour);
	set_field_buffer(f[5],0,n);
	snprintf(n,3,"%02d",u->tm_min);
	set_field_buffer(f[6],0,n);
	u = gmtime(&t.de);
	snprintf(n,3,"%02d",(u->tm_mon)+1);
	set_field_buffer(f[7],0,n);
	snprintf(n,3,"%02d",u->tm_mday);
	set_field_buffer(f[8],0,n);
	snprintf(n,5,"%04d",u->tm_year+1900);
	set_field_buffer(f[9],0,n);
	snprintf(n,3,"%02d",u->tm_hour);
	set_field_buffer(f[10],0,n);
	snprintf(n,3,"%02d",u->tm_min);
	set_field_buffer(f[11],0,n);
	set_field_buffer(f[12],0,t.desc.c_str());
	rnum = t.rnum;
}
int nRecord::exportr(Record &R){
	/**
	 * Assigns field values to an existing record.
	 */
	R.uid = string(field_buffer(f[0],0),trimWS(field_buffer(f[0],0),10));
	R.code = string(field_buffer(f[1],0),trimWS(field_buffer(f[1],0),5));
	R.desc = string(field_buffer(f[12],0),trimWS(field_buffer(f[12],0),12));
	R.ds = get_start_time();
	R.de = get_end_time();
	return rnum;
}

mainMenu::mainMenu(){
	op[0] = new_item("(L)oad Database","Load a database for editing.");
	op[1] = new_item("(N)ew Database","Create a new database for editing.");
	op[2] = new_item("E(x)it Program","Exit the program.");
	op[3] = new_item(" -","Database commands");
	op[4] = new_item("(I)nsert Record","Insert a new record.");
	op[5] = new_item("(V)iew Record","View a record.");
	op[6] = new_item("(E)dit Record","Edit a record.");
	op[7] = new_item("(D)elete Record","Delete a record.");
	op[8] = new_item("(C)lose Database","Close a database without saving.");
	op[9] = new_item("(S)ave Database","Save changes to database.");
	op[10] = 0;
	more_ops(0);
	M = new_menu(op);
	menu_opts_off(M,O_NONCYCLIC);
	set_menu_back(M,COLOR_PAIR(1));
	set_menu_fore(M,COLOR_PAIR(2)|A_STANDOUT);
	set_menu_grey(M,A_DIM);
	//keys['L']=keys['l']=0;
	//keys['N']=keys['n']=1;
	//keys['X']=keys['x']=2;
	//keys['I']=keys['i']=4;
	//keys['V']=keys['v']=5;
	//keys['E']=keys['e']=6;
	//keys['D']=keys['d']=7;
	//keys['C']=keys['c']=8;
	//keys['S']=keys['s']=9;
	char A[] = "LNXIIVEDCS"; // I is mapped twice to skip index 3.
	for (unsigned i=0;i<10;++i) keys[A[i]]=keys[A[i]+32]=i;
}
int mainMenu::has_op(char ch){
	std::map<char,int>::const_iterator it = keys.find(ch);
	if (it==keys.end()) return -1;
	return it->second;
}
int mainMenu::has_op_sc(char ch){
	int i;
	switch (ch){
		case 'L': case 'l': i=0; break;
		case 'N': case 'n': i=1; break;
		case 'X': case 'x': i=2; break;
		case 'I': case 'i': i=4; break;
		case 'V': case 'v': i=5; break;
		case 'E': case 'e': i=6; break;
		case 'D': case 'd': i=7; break;
		case 'C': case 'c': i=8; break;
		case 'S': case 's': i=9; break;
		default: return -1; break;
	}
	return (item_opts(op[i]) & O_SELECTABLE) ? i : -1;
}


void mainMenu::more_ops(bool i){
	int o = O_SELECTABLE & i;
	for (unsigned i=3;i<10;++i){
		set_item_opts(op[i],o);
	}
}
mainMenu::~mainMenu(){
	for (unsigned i=0;i<10;++i){
		free_item(op[i]);
	}
	free_menu(M);
}
int mainMenu::run(){
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	post_menu(M);
	set_current_item(M,op[0]);
	int ch;
	int r = 1;
	int c;
	while (r==1){
		ch = getch();
		switch (ch){
			case KEY_DOWN:
				if (item_opts(op[item_index(current_item(M))+1]) & O_SELECTABLE){
					menu_driver(M,REQ_DOWN_ITEM);
				}
				break;
			case KEY_UP:
				if (item_index(current_item(M))>0 && item_opts(op[item_index(current_item(M))-1]) & O_SELECTABLE){
					menu_driver(M,REQ_UP_ITEM);
				}
				break;
			case 10: //< ENTER
				c = item_index(current_item(M));
				if (c!=3){ r = 0; }
				break;
			default:
				c = has_op_sc(ch);
				if (c!=-1){ r = 0; }
				//if (c!=-1 && c!=3 && (item_opts(op[c]) & O_SELECTABLE)){ r = 0; }
				break;
		}
	}
	unpost_menu(M);
	echo();
	return c;
}

int prompt_rnum(){
	/**
	 * Prompts user for a row number of max-len 9.
	 * If user does not enter '\', output is
	 * guaranteed to be atleast 0. (based on atoi)
	 */
	char s[11] = "~";
	int r = 1;
	while (r==1){
		mvprintw(LINES-1,0,"RNUM:");
		mvgetnstr(LINES-1,5,s,10);
		if (s[0]!='~') return atoi(s);
		if (s[0]!='\\') return -2;
	}
	return -1;
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
	/**
	 * Prompts user for a filename.
	 * ATM this file is not checked by ui and relies on db.
	 */
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
	/**
	 * Show the results of a sqlite3 statement on the mainscreen (stdscr)
	 * Shows upto 25,terminal line count, or rowcount. Whichever is lesser.
	 * The format currently is PRINTNUMBER UID CODE START_TIME DURATION
	 * Where duration is end_time-start_time.
	 * Which means ATM that the statement cannot be arbitary but must
	 * have uid,start_time,end_time,code at indices 0 to 3.
	 * Bound to change.
	 */
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
