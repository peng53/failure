#ifndef FLOOD_ALG
#define FLOOD_ALG
#include <cstdlib> // size_t ??
#include <functional>
void rscanline(char *B,unsigned width,size_t bsize,char newcol,unsigned a,unsigned b,unsigned pt, std::function<void (unsigned,unsigned,int)> snitch);
int scanlinefill(char* board,unsigned width,unsigned height,char newchar,std::function<void (unsigned,unsigned,int)> snitch);
int scanlinefill_new(char* board,unsigned width,unsigned height,char newchar,std::function<void (unsigned,unsigned,int)> snitch);
int r_qfill(char* B,unsigned width,size_t bsize,char newcol,unsigned pt, std::function<void (unsigned,unsigned,int)> snitch);
int qfill(char* board,unsigned width,unsigned height,char newcol, std::function<void (unsigned,unsigned,int)> snitch);

#endif
