#include <ncurses.h>
#include <cstdlib>
#include <cstring>
#include "db.cpp"

int valid_num(char *s,size_t l,unsigned int &n);
void record_creation_win(WINDOW* W);
int build_a_record(unsigned short Y,unsigned short X,TRecord& TT);
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t &s_l,size_t M);
bool valid_wrap(WINDOW *W,unsigned short Y,unsigned short X,char *s,size_t M,unsigned int &n);