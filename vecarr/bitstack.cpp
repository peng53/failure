#include <vector>
#include <iostream>
#include <bitset>

class BitStk {
	private:
		std::vector<uint32_t> data;
		const size_t bsz = 32;
		size_t left, node;
	public:
		BitStk(){
			data.emplace_back(0); //set up first node.
			node = 0; // set current node to 0
			left = bsz; // set 'left' counter to size
		}
		void push(uint32_t x,size_t n){
			while (left<n){
				data[node] <<= left;
				data[node] |= (x>>(n-left)); // AND the first (n-left) bits
				x &= (1<<(n-left))-1; // wipe those bits
				n -= left; // decrease # by bits just added
				if (node+1==data.size()){
					data.emplace_back(0); // make room since current node is full
				}
				++node;
				left = bsz; // reset left to 32 because new node
			}
			if (left>=n){ // either pushing bits or finishing above
				data[node] <<= n; // make room
				data[node] |= (x & (1<<n)-1); // AND only the last n bits
				left -= n; // correct 'left' counter
			}
		}
		uint32_t top(size_t n){
			size_t tleft = left;
			size_t tnode = node;
			uint32_t v = 0;
			size_t place = 0;
			while (bsz-tleft<n){ // if current node contains less than n bits
				v |= (data[tnode] & (1<<(bsz-tleft))-1)<<place;
				// get last '32-left' bits of 'node'
				// move them up 'place' places
				// OR them into v
				place += bsz-left;
				n -= bsz-tleft;
				--tnode; // select previous node
				tleft = 0; // reset 'left' counter
			}
			v |= (data[tnode] & (1<<(n))-1)<<place;
			return v;
		}
		uint32_t top_alt(size_t n){
			// Unrolls top a bit by seperating to three parts:
			// End (first c bits of current node)
			// Middle (entire nodes (b*bsz)
			// Begin (last a bits perceding middle but not filling
			//		node.
			// It is possible that top_alt will only return a 'begin'
			// if all n bits are available from the top node.
			size_t tnode = node;
			size_t place = 0;
			uint32_t v = 0;
			if (bsz-left<n){ // Take what's on the current node.
				v = data[tnode];
				place += bsz-left;
				n -= bsz-left;
				--tnode;
			}
			while (bsz<n){ // Take entire nodes
				// We don't need 'left' here because its always going
				// to be 'bsz' bits taken.
				v |= data[tnode] << place;
				n -= bsz;
				place += bsz;
				--tnode;
			}
			// AND the remaining while masking only needed.
			v |= (data[tnode] & (1<<(n))-1)<<place;
			return v;
		}
		void pop(size_t n){
			/*
			while (bsz-left<n){
				data[node] >>= bsz-left; // pop what bits we can from this node
				n -= bsz-left; // decr what bits were already popped
				--node; // move back a node as we clear it
				left = 0; // reset 'left' counter to # of left on prev node
			}
			*/
			while (bsz-left<n){
				data[node] = 0;
				n -= bsz-left;
				--node;
				left = 0;
			}
			data[node] >>= n; // clear leftover bits
			left += n; // increase 'left' by the bits we popped
		}
		size_t bits(){
			return (node*bsz)+(bsz-left);
		}
		void repr(){
			for (auto n : data){
				std::cout << std::bitset<32>(n) << '\n';
			}
		}
};

int main(){
	BitStk my_stack;
	uint32_t my_vals[] = 	{15,	1,	455,	25,	10};
	size_t my_vals_bs[] = 	{4,		2,	9,		16,	16};
	for (size_t s=0;s<5;++s){
		my_stack.push(my_vals[s],my_vals_bs[s]);
		std::cout << my_stack.top(my_vals_bs[s]) << '\n';
	}
	my_stack.repr();
	for (size_t s=5;s>0;--s){
		std::cout << my_stack.top_alt(my_vals_bs[s-1]) << '\n';
		my_stack.pop(my_vals_bs[s-1]);
	}
	return 0;
}
