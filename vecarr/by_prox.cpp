/**
 * by_prox.cpp is a look into the double subscript operator'[][]'
 * for indexing an class (e.g. multi-dim). This is done here
 * by using a 'proxy' object: indexing the initial object with
 * a base array (1d) returns an object consisting of a ptr to the
 * row in question, which can then be indexed by the column, leading
 * to the cell.
 * This file uses a fixed sixed array of ints. Ideally the array should
 * be replaced by a vector<T> (or templated new[]).
 */

#include <array> // isn't actually used?
#include <iostream> // for cout
#include <algorithm> // for std::fill
#include <random> // isn't actually used?

struct VRow {
	size_t C;
	int* data;
	VRow(int* d,size_t c): data(d), C(c){}
	int& operator[](size_t c){
		if (c>=C){
			throw std::exception();
		}
		return *(data+c);
	}
};
struct Board {
	size_t R = 20,C = 22;
	int data[440];
	Board(){
		std::fill(std::begin(data),std::end(data),20);
	}
	VRow operator[](size_t r){
		if (r>=R){
			throw std::exception();
		}
		return VRow(data,C);
	}
};

int main(){
	Board myboard;
	std::cout << myboard[19][21] << '\n';
	myboard[19][21] = 26;
	std::cout << myboard[19][21] << '\n';
	return 0;
}
