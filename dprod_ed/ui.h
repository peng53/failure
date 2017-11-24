#ifndef D_UI
#define D_UI
#include <ncurses.h>
#include <form.h>
#include "db.h"

time_t get_start_time(FIELD* f[15]);
time_t get_end_time(FIELD* f[15]);
void prepare_fields(FIELD *field[15]);
void populate_fields(FIELD *field[15],Record &t);
void clean_up_rec_form(FORM *form,FIELD *field[15]);
void dress_rec_win(WINDOW* W,int rnum);
int prompt_rnum();
int make_rec_win(unsigned Y,unsigned X,Record &t);
int valid_str(char *s);
int getAfileName(char *s);
int show_results(sqlite3_stmt* s);

#endif
