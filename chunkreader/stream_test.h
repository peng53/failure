#ifndef STREAM_TEST
#define STREAM_TEST
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <string>

using std::ios_base;

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
				//std::cout << "got : " << E << '\n';
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
			if (I+1<=E){
				++I;
			} else if (good){
				feed();
			}
			return *this;
		}
		bool has_data(){
			return I<E;
		}
};
#endif
