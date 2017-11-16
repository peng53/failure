#ifndef D_UI
#define D_UI
#include "ui.h"

int valid_num(char *s,size_t l,unsigned int &n){
	/**
	Changes string to unsigned int if valid.
	Return codes:
	0: success
	1: no/bad input
	2: special input
	*/
	if (s[0]=='\n' && s[1]=='\0'){ return 1; }
	if (s[0]=='\\'){ return 2; }
	for (unsigned short i=0;i<l && s[i]!='\n';++i){
		if (s[i]<48 || s[i]>57){ return 1; }
	}
	n = atoi(s);
	return 0;
}
int valid_str(char *s,size_t l,size_t &n){
	/**
	Checks if string has length and counts length.
	Return codes:
	0: success
	1: no/bad input
	2: special input
	*/
	if (s[0]=='\n' && s[1]=='\0'){ return 1; }
	if (s[0]=='\\'){ return 2; }
	char *e = strchr(s,'\n');
	n = (e==NULL) ? l : (size_t)(e-s);
	return 0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t &s_l,size_t M){
	int r = 1;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		r = valid_str(s,M,s_l);
	}
	return r==0;
}
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t M,unsigned int &n){
	int r = 1;
	while (r==1){
		mvwgetnstr(W,Y,X,s,M);
		r = valid_num(s,M,n);
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
	mvwhline(W,14,3,ACS_BLOCK,25);
	mvwhline(W,15,3,ACS_BLOCK,25);
	mvwhline(W,16,3,ACS_BLOCK,25);
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

int build_a_record(unsigned short Y,unsigned short X,TRecord &TT){
	WINDOW* a_record = newwin(20,31,Y,X);
	record_creation_win(a_record);
	wattron(a_record,A_REVERSE);
	int r = 1;
	//~ while (r==1){
		//~ mvwgetnstr(a_record,3,6,TT.uid,10); ///< uid
		//~ r = valid_str(TT.uid,10,TT.uid_l);
	//~ }
	//~ if (r==2){ return 1; }
	//~ r = 1;
	//~ while (r==1){
		//~ mvwgetnstr(a_record,3,23,TT.code,5); ///< code
		//~ r = valid_str(TT.code,5,TT.code_l);
	//~ }
	if (!valid_wrap(a_record,3,6,TT.uid,TT.uid_l,10) || !valid_wrap(a_record,3,23,TT.code,TT.code_l,5)) { return 1; }
	if (!valid_wrap(a_record,7,3,TT.desc,2,TT.date[0])){ return 1; }
	if (!valid_wrap(a_record,7,6,TT.desc,2,TT.date[1])){ return 1; }
	if (!valid_wrap(a_record,7,9,TT.desc,4,TT.date[2])){ return 1; }
	if (!valid_wrap(a_record,7,14,TT.desc,2,TT.date[3])){ return 1; }
	if (!valid_wrap(a_record,7,17,TT.desc,2,TT.date[4])){ return 1; }
	//~ for (size_t i=0;i<2;++i){ ///< d1mth & d1d
		//~ r = 1;
		//~ while (r==1){
			//~ mvwgetnstr(a_record,7,3+(i*3),TT.desc,2);
			//~ r = valid_num(TT.desc,2,TT.date[i]);
		//~ }
		//~ if (r==2){ return 1; }
	//~ }
	//~ r = 1;
	//~ while (r==1){ ///< d1yr
		//~ mvwgetnstr(a_record,7,9,TT.desc,4);
		//~ r = valid_num(TT.desc,4,TT.date[2]);
	//~ }
	//~ if (r==2){ return 1; }
	//~ for (size_t i=3;i<5;++i){ ///< d1hr & d1mm
		//~ r = 1;
		//~ while (r==1){ ///< d1mth & d1d
			//~ mvwgetnstr(a_record,7,14+((i-3)*3),TT.desc,2);
			//~ r = valid_num(TT.desc,2,TT.date[i]);
		//~ }
		//~ if (r==2){ return 1; }
	//~ }
	//~ mvwgetnstr(a_record,7,6,desc,2); ///< d1d
	//~ mvwgetnstr(a_record,7,9,desc,4); ///< d1yr
	//~ mvwgetnstr(a_record,7,14,desc,2); ///< d1hh
	//~ mvwgetnstr(a_record,7,17,desc,2); ///< d1mm
	//~ mvwgetnstr(a_record,11,3,desc,2); ///< d2mth
	//~ mvwgetnstr(a_record,11,6,desc,2); ///< d2d
	//~ mvwgetnstr(a_record,11,9,desc,4); ///< d2yr
	//~ mvwgetnstr(a_record,11,14,desc,2); ///< d2hh
	//~ mvwgetnstr(a_record,11,17,desc,2); ///< d2mm
	//~ mvwgetnstr(a_record,14,3,desc,25); ///< desc-l1
	//~ mvwgetnstr(a_record,15,3,desc,25); ///< desc-l2
	//~ mvwgetnstr(a_record,16,3,desc,25); ///< desc-l3
	wattroff(a_record,A_REVERSE);
	delwin(a_record);
	return 0;
}

#endif