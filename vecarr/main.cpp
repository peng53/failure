#include <vector>
#include <iostream>
#include <queue>
#include <tuple>

template <class T>
class VecArr {
	private:
		T* base_arr;
		std::vector<T*> wrap_arr;
	public:
		const size_t rows, cols, size;
		VecArr(const size_t col_count,const size_t row_count):size(col_count*row_count), cols(col_count), rows(row_count){
			base_arr = new T[size];
			wrap_arr.reserve(row_count);
			for (size_t n=0;n<size;n+=col_count){
				wrap_arr.emplace_back(&base_arr[n]);
			}
		}
		void set_val(const T& newval){
			for (size_t s=0;s<size;++s){
				base_arr[s] = newval;
			}
		}
		void set_row(const size_t row,const T* const newvals){
			/**
			 * Note: |newvals| should be >= cols else problems
			 */
			for (size_t c=0;c<cols;++c){
				wrap_arr[row][c] = newvals[c];
			}
		}
		VecArr(const size_t col_count,const size_t row_count,const T& init_val): VecArr(col_count,row_count){
			set_val(init_val);
		}
		VecArr(const size_t col_count,const size_t row_count,const T* const init_row): VecArr(col_count,row_count){
			/*
			 * Note: |init_row| must be >=col_count unless you want garbage values
			 */
			for (size_t r=0;r<rows;++r){
				set_row(r,init_row);
			}
		}
		~VecArr(){
			delete[] base_arr;
		}
		T* operator[](const size_t row){
			return wrap_arr[row];
		}
		T** begin(){
			return &(wrap_arr[0]);
		}
		T** end(){
			return &(wrap_arr[rows]);
		}
};

template <class T>
size_t expand_sl_left(VecArr<T> &va,size_t y,size_t x,const T to_match){
	while (x>0 && va[y][x-1]==to_match){
		--x;
	}
	return x;
}
template <class T>
size_t expand_sl_right(VecArr<T> &va,size_t y,size_t x,const T to_match){
	while (x<va.cols-1 && va[y][x+1]==to_match){
		++x;
	}
	return x;
}

template <class T>
void scanlinefill(VecArr<T> &va,size_t y,size_t x0,size_t x1,const T new_color){
	T old_color = va[y][x0];
	if (old_color==new_color){
		return;
	}
	std::queue<std::tuple<size_t,size_t,size_t>> q;
	q.emplace(y,x0,x1);
	bool has_upper = 0, has_lower = 0;
	size_t upper_x0, upper_x1, lower_x0, lower_x1;
	size_t y_max = va.rows-1;
	while (!q.empty()){
		std::tie(y,x0,x1) = q.front();
		q.pop();
		if (va[y][x0]!=old_color){
			continue;
		}
		for (;x0<=x1;++x0){
			va[y][x0] = new_color;
			if (y>0 && va[y-1][x0]==old_color){
				if (!has_upper){
					has_upper = 1;
				} else if (upper_x1+1==x0){
					++upper_x1;
				} else {
					q.emplace(y-1,expand_sl_left(va,y-1,upper_x0,old_color),upper_x1);
					upper_x0 = upper_x1 = x0;
				}
			}
		}
	}
}

int main(){
	size_t rows = 3;
	size_t cols = 10;
	float test_row [10] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.10};
	VecArr<float> my_vec_array(cols,rows,test_row);
	for (size_t r=0;r<my_vec_array.rows;++r){
		std::cout << "ROW " << r << ':';
		for (size_t c=0;c<my_vec_array.cols;++c){
			std::cout << my_vec_array[r][c] << ' ';
			my_vec_array[r][c] = (r+1.5)*(c+2);
		}
		std::cout << '\n';
	}
	std::cout << '\n';
	for (size_t r=0;r<my_vec_array.rows;++r){
		std::cout << "ROW " << r << ':';
		for (size_t c=0;c<my_vec_array.cols;++c){
			std::cout << my_vec_array[r][c] << ' ';
		}
		std::cout << '\n';
	}
	for (size_t s=0;s<10;++s){
		test_row[s] *= 10;
	}
	my_vec_array.set_row(1,test_row);
	for (auto tp : my_vec_array){
		std::cout << tp << '\n';
		for (size_t c=0;c<my_vec_array.cols;++c){
			std::cout << tp[c] << ' ';
		}
		std::cout << '\n';
	}
	return 0;
}
