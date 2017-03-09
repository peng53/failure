//#include <iostream>
#include <sstream>
#include <list>
#include <stack>
#include <random>
#include <string>
//#include "b_inc.cpp"

using std::string;
using std::vector;
using std::stack;
using std::ostream;
using std::list;

//using std::cout;

std::mt19937 RNG(time(0));
std::binomial_distribution<bool> coin(1,0.5);

struct ST {
	unsigned L = 0, W = 1;
	string * I = nullptr;
	string * D = nullptr;
	ST(string &s) : I(&s){}
	ST(string &s,unsigned c) : L(c),I(&s){}
	ST(string &s,string &d,unsigned r) : W(r),I(&s),D(&d){}
	ST(string &s,unsigned c,string &d,unsigned r) : L(c),W(r),I(&s),D(&d){}
};
ostream& operator<<(ostream& out,ST &P);
class PartedString {
	private:
		vector<string> literals;
		list<ST> parts;
		stack<string*> emerg;
		friend ostream& operator<<(ostream& out,PartedString &PS);
	public:
		size_t lits_size() const{
			return literals.size();
		}
		void add_literal(string s){
			literals.push_back(s);
		}
		string& get_lit(unsigned i){
			return literals[i];
		}
		void add_part(unsigned I){
			parts.emplace_back(literals[I]);
		}
		void add_part(const string &s){
			emerg.push(new string {s});
			//cout<< "made " << emerg.top()<<'\n';
			parts.emplace_back(*emerg.top());
		}
		void add_part_2(unsigned I, unsigned L){
			parts.emplace_back(literals[I],L);
		}
		void add_part_3(unsigned I, unsigned D, unsigned R){
			parts.emplace_back(literals[I],literals[D],R);
		}
		void add_part_4(unsigned I, unsigned L, unsigned D, unsigned W){
			parts.emplace_back(literals[I],L,literals[D],W);
		}
		~PartedString(){
			for(;!emerg.empty();emerg.pop()){ /*cout<< "deleting " << emerg.top()<<'\n';*/delete emerg.top();}\
		}
};

ostream& operator<<(ostream& out,PartedString &PS);

//const char* stir_mix(string &C, const unsigned L, const unsigned W, const char *S);
//extern "C" const char* instant_coffee(const char *C, const unsigned L, const unsigned W, const char *S);
//extern "C" const char* plen(const char *I,char *E, const unsigned L, const unsigned W, const char *S);
