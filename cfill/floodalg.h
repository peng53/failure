#ifndef FLOOD_ALG
#define FLOOD_ALG
#include <cstdlib> // size_t ??

void rscanline(char *B,unsigned width,size_t bsize,char newcol,unsigned a,unsigned b);
int scanlinefill(char* board,unsigned width,unsigned height,char newchar);
int r_qfill(char* B,unsigned width,size_t bsize,char newcol,unsigned pt);
int qfill(char* board,unsigned width,unsigned height,char newcol);

#endif
