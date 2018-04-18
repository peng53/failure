#include <vector>
#include <iostream>
#include <bitset>

void push(std::vector<uint32_t> &stk,size_t &left,size_t &node,uint32_t x,size_t n){
	while (left<n){
		stk[node] <<= left; // make room for the bits
		stk[node] |= (x>>(n-left)); // AND the first (n-left) bits
		x &= (1<<(n-left))-1; // wipe those bits
		n -= left; // decrease # by bits just added
		if (node+1==stk.size()){
			stk.emplace_back(0); // make room since current node is full
		}
		++node;
		left = 32; // reset left to 32 because new node
	}
	if (left>=n){ // either pushing bits or finishing above
		stk[node] <<= n; // make room
		stk[node] |= (x & (1<<n)-1); // AND only the last n bits
		left -= n; // correct 'left' counter
	}
}
uint32_t top(std::vector<uint32_t> &stk,size_t left,size_t node,uint32_t n){
	uint32_t v = 0;
	size_t place = 0;
	while (32-left<n){ // if current node contains less than n bits
		v |= (stk[node] & (1<<(32-left))-1)<<place;
		// get last '32-left' bits of 'node'
		// move them up 'place' places
		// OR them into v
		place += 32-left;
		n -= 32-left;
		--node; // select previous node
		left = 0; // reset 'left' counter
	}
	v |= (stk[node] & (1<<(n))-1)<<place;
	return v;
}

int main(){
	std::vector<uint32_t> my_stack;
	my_stack.emplace_back(0);
	size_t left = 32;
	size_t node = 0;
	uint32_t a = 15; // repr by 4 bits 1111
	uint32_t b = 1; // repr by 2 bits 01
	uint32_t c = 455; // repr by 9 bits 111000111
	uint32_t d = 25; // repr by 16 bits
	uint32_t e = 10; // repr by 16 bits
	//my_stack.back() <<= 4;
	//std::cout << (my_stack.back() | (a & (1<<4)-1));
	push(my_stack,left,node,a,4);
	std::cout << top(my_stack,left,node,4) << '\n';
	push(my_stack,left,node,b,2);
	std::cout << top(my_stack,left,node,2) << '\n';
	push(my_stack,left,node,c,9);
	std::cout << top(my_stack,left,node,9) << '\n';
	push(my_stack,left,node,d,16);
	std::cout << top(my_stack,left,node,16) << '\n';
	push(my_stack,left,node,e,16);
	std::cout << top(my_stack,left,node,16) << '\n';
	for (auto t : my_stack){
		std::cout << t << ' ';
	}
	std::cout << '\n';
	for (auto t : my_stack){
		std::cout << std::bitset<32>(t) << ' ';
	}
	std::cout << '\n';

	return 0;
}
