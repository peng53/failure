#ifndef VECARR
#define VECARR
#include <ostream>
template <class T>
class VecArr {
	private:
		T* base_arr;
		//std::vector<T*> wrap_arr;
		T** wrap_arr;
	public:
		const size_t rows, cols, size;
		VecArr(const size_t col_count,const size_t row_count);
		void set_val(const T& newval);
		void set_vals(const size_t valcnt,const T* const newvals);
		void set_row(const size_t row,const T* const newvals);
		VecArr(const size_t col_count,const size_t row_count,const T& init_val);
		VecArr(const size_t col_count,const size_t row_count,const T* const init_row);
		~VecArr();
		T* operator[](const size_t row);
		T* operator[](const size_t row) const;
		T** begin();
		T** end();
	template <class TT>
	friend std::ostream& operator <<(std::ostream &out,VecArr<TT> &va);
};

template <class TT>
std::ostream& operator <<(std::ostream &out,VecArr<TT> &va);

template <class T>
size_t expand_sl_left(const VecArr<T> &va,const size_t y,size_t x,const T to_match);
template <class T>
size_t expand_sl_right(const VecArr<T> &va,const size_t y,size_t x,const T to_match);
template <class T>
size_t expand_sl_i(const VecArr<T> &va,const size_t y,size_t x,const T to_match,const int dir);

template <class T>
void scanlinefill(VecArr<T> &va,size_t y,size_t x0,size_t x1,const T new_color);
#endif
