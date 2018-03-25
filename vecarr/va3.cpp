#include <vector>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <tuple>

template <class T>
class VecArr3 {
	private:
		T* base_arr;
		const size_t off1;
	public:
		const size_t layers, rows, cols, size;
		void set_val(const T val){
			for (size_t i=0;i<size;++i){
				base_arr[i] = val;
			}
		}
		VecArr3(const size_t _dim1, const size_t _dim2, const size_t _dim3):
			layers(_dim1),
			rows(_dim2),
			cols(_dim3),
			size(_dim1*_dim2*_dim3),
			off1(_dim2*_dim3){
			base_arr = new T[size];
		}
		VecArr3(const size_t _dim1, const size_t _dim2, const size_t _dim3, const T val):
			VecArr3(_dim1,_dim2,_dim3){
			set_val(val);
		}
		~VecArr3(){
			delete[] base_arr;
		}
		T& at(const size_t z,const size_t y=0,const size_t x=0){
			return base_arr[z*off1+y*cols+x];
		}
		T& xyz(const size_t x,const size_t y,const size_t z){
			return base_arr[z*off1+y*cols+x];
		}
		T& operator[](std::tuple<const size_t,const size_t,const size_t> zyx){
			return base_arr[std::get<0>(zyx)*off1+std::get<1>(zyx)*cols+std::get<2>(zyx)];
		}
		template<class TT>
		friend std::ostream& operator<<(std::ostream& out,VecArr3<TT>& va);
};
template <class TT>
std::ostream& operator<<(std::ostream& out,VecArr3<TT>& va){
	size_t a = 0;
	for (size_t layer=0;layer<va.layers;++layer){
		out << "Layer: " << layer << '\n';
		for (size_t r=0;r<va.rows;++r){
			for (size_t c=0;c<va.cols;++c){
				out << va.base_arr[a++] << out.fill();
			}
			out << '\n';
		}
	}
	return out;
}

int main(){
	VecArr3<int> va(3,2,4,-1);
	size_t a = 0;
	for (size_t layer=0;layer<va.layers;++layer){
		std::cout << "Layer: " << layer << '\n';
		for (size_t r=0;r<va.rows;++r){
			for (size_t c=0;c<va.cols;++c){
				std::cout << va.at(layer,r,c) << ' ';
				va.at(layer,r,c) = a++;
			}
			std::cout << '\n';
		}
	}
	std::cout << va << '\n';
	return 0;
}
