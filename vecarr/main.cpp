#include <vector>
#include <iostream>
#include <cstdlib>

template <class T>
class VecArr {
	private:
		T* base_arr;
		size_t size;
	public:
		std::vector<T*> wrap_arr;
		VecArr(size_t col_count,size_t row_count):size(col_count*row_count){
			base_arr = (T*)malloc(size*sizeof(T));
			for (size_t n=0;n<size;n+=col_count){
				wrap_arr.emplace_back(&base_arr[n]);
			}
		}
		VecArr(size_t col_count,size_t row_count,T init_val): VecArr(col_count,row_count){
			for (size_t s=0;s<size;++s){
				base_arr[s] = init_val;
			}
		}
		VecArr(size_t col_count,size_t row_count,T* init_row): VecArr(col_count,row_count){
			/*
			 * Note: |init_row| must be >=col_count
			 */
			size_t s = 0;
			for (size_t r=0;r<row_count;++r){
				for (size_t c=0;c<col_count;++c){
					base_arr[s] = init_row[c];
					++s;
				}
			}
		}
		~VecArr(){
			free(base_arr);
		}
		T* operator[](size_t row){
			return wrap_arr[row];
		}

};

int main(){
	size_t rows = 3;
	size_t cols = 10;
	float test_row [10] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10.10};
	VecArr<float> my_vec_array(cols,rows,test_row);
	for (size_t r=0;r<rows;++r){
		std::cout << "ROW " << r << ':';
		for (size_t c=0;c<cols;++c){
			std::cout << my_vec_array[r][c] << ' ';
			my_vec_array[r][c] = (r+1.5)*(c+2);
		}
		std::cout << '\n';
	}
		std::cout << '\n';
	for (size_t r=0;r<rows;++r){
		std::cout << "ROW " << r << ':';
		for (size_t c=0;c<cols;++c){
			std::cout << my_vec_array[r][c] << ' ';
		}
		std::cout << '\n';
	}
	return 0;
}
