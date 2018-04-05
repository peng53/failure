#include <random>
#include <iostream>
#include <bitset>
#include <cstdint>

int main(){

	// if i want 12 blocks across
	unsigned width = 12;
	// thats 12*4 bits, or 48.
	// I want 8bits per value, so uint8_t should be more correct than unsigned char.
	size_t arr_width = (width*4)/8;
	// i need 6 chars across.
	// if arr_width is calculated like this, it needs to be >=2
	size_t arr_height = 1;
	// arr_height is 1
	//unsigned char* arr = new unsigned char[arr_width*arr_height];
	uint8_t* arr = new uint8_t[arr_width*arr_height];
	// set blocks to random?
	// try 2 block at a time..
	std::default_random_engine gen;
	//std::uniform_int_distribution<unsigned char> r_cell(0,15);
	std::uniform_int_distribution<uint8_t> r_cell(0,15);
	// 0 to 15 is range between b0000 and b1111
	//unsigned char r;
	uint8_t r;
	std::cout << "            NESW\n";
	for (size_t x=0;x<arr_width;++x){
		//arr[x] = 0;
		//// 'blank' current 2 cells
		r = r_cell(gen);
		// to check correctness, we will output the bitset as well.
		std::cout << "Random r = " << (unsigned)r << ' ' << std::bitset<4>(r) << '\n';
		arr[x] = r<<4;
		// shift that 4 bits, making room for 4 at the end
		r = r_cell(gen);
		std::cout << "Random s = " << (unsigned)r << ' ' << std::bitset<4>(r) << '\n';
		arr[x] |= r;
		// set last 4 bits
	}
	// lets print out the blocks..
	// lets do it as 3x3 blocks
	for (size_t x=0;x<arr_width;++x){
		// this is the top portion (0,0) to (0,2)
		std::cout << '#';
		// the bit we need to check is the North bit
		// of the 'even' block. the 8th bit is it
		// to access this bit, we shift 8-1 places;
		if ((arr[x] >> 7) & 1U){
			// is there an opening?
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
		std::cout << '#' << '$';
		// now to print the north bit of the odd block
		// this is the 4th bit, so 3 shifts
		if ((arr[x] >> 3) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '$';
		}
		std::cout << '$';
	}
	std::cout << '\n';
	for (size_t x=0;x<arr_width;++x){
		// middle portion (1,0) to (1,2);
		// we need to check West Even first.
		// West is the last bit so 1+4-1
		if ((arr[x] >> 4) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
		// ATM we will leave the middle (1,1) empty
		// but ideally we have to check if any of the bits
		// in the even block is set.
		std::cout << ' ';
		// Now the east bit (even), 3rd so 3+4-1
		if ((arr[x] >> 6) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
		// Repeat for odd block, (without -4)
		// so..
		if ((arr[x] >> 0) & 1U){ // note this is same as arr[x] & 1U; e.g. checking last bit
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
		std::cout << ' ';
		if ((arr[x] >> 2) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
	}

	std::cout << '\n';
	for (size_t x=0;x<arr_width;++x){
		// this is the bottom portion (2,0) to (2,2)
		std::cout << '#';
		// first check 'even' south, the 2nd bit
		// or 2+4-1, makes sense since it is 2 away from north
		if ((arr[x] >> 5) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '#';
		}
		std::cout << '#' << '$';
		// once again, 2nd, so 2-1, or 1
		if ((arr[x] >> 1) & 1U){
			std::cout << ' ';
		} else {
			std::cout << '$';
		}
		std::cout << '$';
	}

	delete[] arr;
	return 0;
}
