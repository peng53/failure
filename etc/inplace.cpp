/* inplace.cpp
 * Attempt at a 'fixed sized stack' which is really a
 * vector that gets angry if you try to go over.
 * Looks like I was trying to use template and storing
 * an 'error object' (eo) to be returned if no more objects
 * exist. Although I think it would have been better to use
 * an ::empty function?
 *
 */
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class FStk {
	vector<T> e;
	int cur = -1;
	int size;
	T eo;
	public:
	FStk(int _size, T _eo){
		if (_size>0){
			size = _size;
			eo = _eo;

		}
	}
	T pop(){
		if (cur!=-1) return e[cur--];
		else return eo;
	}
	bool push(T v){
		if (cur<size){
			cur++;
			e.push_back(v);
			return 0;
		}
		return 1;
	}
	int maxsize(){
		return size;
	}

};
template <class T>
void merge(vector<T> A, unsigned int L, unsigned int C, unsigned int R, FStk<T> f){
}

int main(){
	FStk<int>t (10,24);
	for (int u=0;u<4;++u){
		t.push(u);
	}
	for (int u=0;u<10;++u){
		cout << t.pop() << '\n';
	}
	return 0;
}
