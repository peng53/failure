#ifndef STREAM_TEST
#define STREAM_TEST
#include <iostream>
#include <fstream>

class Obj {
	private:
		std::streambuf* t;
		char *ch;
		size_t I, E;
		const size_t M;
	public:
		bool good;
		Obj(std::streambuf* sb,const size_t csize):
			t(sb),
			ch(new char[csize]),
			I(0),
			E(0),
			M(csize),
			good(true)
		{
			feed();
		}
		void feed(){
			if (good){
				I = 0;
				E = t->sgetn(ch,M-1);
				ch[E] = '\0';
				if (E<M-1){
					good = false;
				}
			} else {
				I = 0;
				E = 0;
			}
		}
		char get(){
			return ch[I];
		}
		Obj& operator++(){
			++I;
			if (!has_data() && good){
				feed();
			}
			return *this;
		}
		bool has_data(){
			return I<E;
		}
};
#endif
