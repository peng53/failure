//#include <vector>
#include <iostream>
#include <queue>
#include <tuple>
#include <ostream>
#include <iomanip>

template <class T>
class VecArr {
	private:
		T* base_arr;
		//std::vector<T*> wrap_arr;
		T** wrap_arr;
	public:
		const size_t size, cols, rows;
		VecArr(const size_t col_count,const size_t row_count):size(col_count*row_count), cols(col_count), rows(row_count){
			base_arr = new T[size];
			wrap_arr = new T*[row_count];
			//wrap_arr.reserve(row_count);
			for (size_t n=0, a=0;n<size;n+=col_count){
				//wrap_arr.emplace_back(&base_arr[n]);
				wrap_arr[a++] = &base_arr[n];
			}
		}
		void set_val(const T& newval){
			for (size_t s=0;s<size;++s){
				base_arr[s] = newval;
			}
		}
		void set_vals(const size_t valcnt,const T* const newvals){
			/**
			 * Sets values of base_array for indices 0 to valcnt-1 to newvals
			 * Required: |newvals| >= valcnt
			 */
			for (size_t i=0;i<valcnt;++i){
				base_arr[i] = newvals[i];
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
			delete[] wrap_arr;
		}
		T* operator[](const size_t row){
			return wrap_arr[row];
		}
		T* operator[](const size_t row) const {
			return wrap_arr[row];
		}
		T** begin(){
			return &(wrap_arr[0]);
		}
		T** end(){
			return &(wrap_arr[rows]);
		}

	template <class TT>
	friend std::ostream& operator <<(std::ostream &out,VecArr<TT> &va);
};

template <class TT>
std::ostream& operator <<(std::ostream &out,VecArr<TT> &va){
	for (auto tp : va){
		//out << tp << '\n'; if class is char, it will print the char*, else a memory location
		for (size_t c=0;c<va.cols;++c){
			out << tp[c] << out.fill();
		}
		out << '\n';
	}
	return out;
}

template <class T>
size_t expand_sl_left(const VecArr<T> &va,const size_t y,size_t x,const T to_match){
	while (x>0 && va[y][x-1]==to_match){
		--x;
	}
	return x;
}
template <class T>
size_t expand_sl_right(const VecArr<T> &va,const size_t y,size_t x,const T to_match){
	while (x<va.cols-1 && va[y][x+1]==to_match){
		++x;
	}
	return x;
}
template <class T>
size_t expand_sl_i(const VecArr<T> &va,const size_t y,size_t x,const T to_match,const int dir){
	size_t lim_x = (dir==-1) ? 0 : va.cols-1;
	while (x!=lim_x && va[y][x+dir]==to_match){
		(dir==-1) ? --x : ++x;
	}
	return x;
}

template <class T>
void scanlinefill(VecArr<T> &va,size_t y,size_t x0,size_t x1,const T new_color){
	T old_color = va[y][x0];
	//std::cout << "old color is " << old_color << '\n';
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
		//std::cout <<"y,x,x1 is: " << y << ' ' << x0 << ' ' << x1 << '\n';
		if (va[y][x0]!=old_color){
			continue;
		}
		for (;x0<=x1;++x0){
			va[y][x0] = new_color;
			if (y>0 && va[y-1][x0]==old_color){
				if (!has_upper){
					has_upper = 1;
					upper_x0 = upper_x1 = x0;
				} else if (upper_x1+1==x0){
					++upper_x1;
				} else {
					//q.emplace(y-1,expand_sl_left(va,y-1,upper_x0,old_color),upper_x1);
					q.emplace(y-1,expand_sl_i(va,y-1,upper_x0,old_color,-1),upper_x1);
					upper_x0 = upper_x1 = x0;
				}
			}
			if (y<y_max && va[y+1][x0]==old_color){
				if (!has_lower){
					has_lower = 1;
					lower_x0 = lower_x1 = x0;
				} else if (lower_x1+1==x0){
					++lower_x1;
				} else {
					//q.emplace(y+1,expand_sl_left(va,y+1,lower_x0,old_color),lower_x1);
					q.emplace(y+1,expand_sl_i(va,y+1,lower_x0,old_color,-1),lower_x1);
					lower_x0 = lower_x1 = x0;
				}
			}
		}
		if (has_upper){
			//q.emplace(y-1,expand_sl_left(va,y-1,upper_x0,old_color),expand_sl_right(va,y-1,upper_x1,old_color));
			q.emplace(y-1,expand_sl_i(va,y-1,upper_x0,old_color,-1),expand_sl_i(va,y-1,upper_x1,old_color,1));
			has_upper = 0;
		}
		if (has_lower){
			//q.emplace(y+1,expand_sl_left(va,y+1,lower_x0,old_color),expand_sl_right(va,y+1,lower_x1,old_color));
			q.emplace(y+1,expand_sl_i(va,y+1,lower_x0,old_color,-1),expand_sl_i(va,y+1,lower_x1,old_color,1));
			has_lower = 0;
		}
	}
}

int main(){
	char test_row[26] =
		"oxxoo"
		"ooxoo"
		"oooxo"
		"xxooo"
		"oooxx"; // 0,0 all should change
	size_t rows = 5;
	size_t cols = 5;
	VecArr<char> my_vec_array(cols,rows);
	my_vec_array.set_vals(25,test_row);
	std::cout << my_vec_array << '\n';
	scanlinefill(my_vec_array,3,2,4,'x');
	std::cout << my_vec_array << '\n';
	std::cout << "\nNon-square matrix test\n";
	char test_row2[29] =
		"xooxxox"
		"xxooxxo"
		"oxoxoxo"
		"ooooxoo";
	rows = 4;
	cols = 7;
	VecArr<char> my_vec_array2(cols,rows);
	my_vec_array2.set_vals(28,test_row2);
	std::cout << my_vec_array2 << '\n';
	scanlinefill(my_vec_array2,2,2,2,'x');
	std::cout << std::setfill(',') << my_vec_array2 << '\n';
	std::cout << std::setfill('\0') << my_vec_array2 << '\n';
	return 0;
}
