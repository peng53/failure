#include <iostream>
#include <vector>
#include <limits>
#include <bitset>
#include <random>
#include <ncurses.h>
#include "mprint.h"

uint32_t mcell(uint32_t block,size_t y,size_t x){
	// Returns a maze cell given a block
	unsigned shift = 8*(y % 4);
	if (x & 1) shift += 4;
	//std::cout << "y,x " << y << ',' << x << ' ' << shift << '\n';
	return (block>>shift) & 0xF;
	// 0xF is equal having only the last 4 bits on
}

uint32_t mblock(uint32_t** arr2d,size_t y,size_t x){
	// Returns a maze block given its coords
	size_t new_x = x/2;
	size_t new_y = y/4;
	return arr2d[new_y][new_x];
}
int main(){
	// Using a 32bit int, I will try to store 8 maze cells.
	// Storing them like this:
	//	AAAA	BBBB
	//	CCCC	DDDD
	//	EEEE	FFFF
	//	GGGG	HHHH
	// This is a 2x4 (WxH) maze. In a 4x4 maze, CCCC DDDD would be
	// adjacent to BBBB on the right.
	std::default_random_engine gen;
	std::uniform_int_distribution<uint32_t> r_cell(0,std::numeric_limits<uint32_t>::max());
	// I will be using an uniform int dist to generate all 8 blocks at once.
	// 0 to  is range between b0000 and b1111
	//uint32_t my_block = r_cell(gen);
	uint32_t my_block = 19088743; // all blocks
	std::cout << my_block << '\n';
	uint32_t temp_cell_a, temp_cell_b;
	size_t x=0;
	for (size_t y=0;y<4;++y){
		temp_cell_a = mcell(my_block,y,x);
		temp_cell_b = mcell(my_block,y,x+1);
		//std::cout << temp_cell_a << ',' << std::bitset<32>(temp_cell_a) << '\n';
		//std::cout << temp_cell_b << ',' << std::bitset<32>(temp_cell_b) << '\n';
		// this is the top portion (0,0) to (0,2)
		std::cout << '#';
		std::cout << (((temp_cell_a>>3) & 1) ? ' ' : '#'); // check for N opening
		std::cout << '#';
		std::cout << '#';
		std::cout << (((temp_cell_b>>3) & 1) ? ' ' : '#'); // check for N opening
		std::cout << '#' << '\n';

		// middle.
		std::cout << (((temp_cell_a) & 1) ? ' ' : '#'); // check for W opening
		std::cout << ' ';
		std::cout << (((temp_cell_a>>2) & 1) ? ' ' : '#'); // check for E opening
		std::cout << (((temp_cell_b) & 1) ? ' ' : '#'); // check for W opening
		std::cout << ' ';
		std::cout << (((temp_cell_b>>2) & 1) ? ' ' : '#'); // check for E opening
		std::cout << '\n';

		//bottom
		std::cout << '#';
		std::cout << (((temp_cell_a>>1) & 1) ? ' ' : '#'); // check for S opening
		std::cout << '#';
		std::cout << '#';
		std::cout << (((temp_cell_b>>1) & 1) ? ' ' : '#'); // check for S opening
		std::cout << '#' << '\n';
	}
	std::cout << "Bits: " << my_block << "\n   _   _   _   _   _   _   _   _\n" << std::bitset<32>(my_block) << '\n';

	initscr();
	for (int y=0;y<4;++y){
		for (int x=0;x<2;++x){
			MazePrint::print_cell(mcell(my_block,y,x),y*3,x*3);
		}
	}
	refresh();
	getch();

	endwin();
	return 0;
}
