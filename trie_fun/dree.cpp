/* Digit tree : precursor to trie_fun
 * Stores integers in a tree where each node can have
 * up to 10 children (digits). Printing seems to return
 * them in reverse order. (123) -> (321)
 */

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Node {
	Node* p[10];
	bool end;
	Node(): end(0){
		for (unsigned i=0;i<10;++i) p[i] = nullptr;
	}
	~Node(){
		for (unsigned i=0;i<10;++i){
			if (p[i]!=nullptr){
				delete p[i];
				p[i]=nullptr;
			}
		}
	}
	void print_ch(vector<unsigned> &buf){
		if (end){
			for (unsigned i=0;i<buf.size();++i) cout << buf[i];
			cout << '\n';
		}
		for (unsigned i=0;i<10;++i){
			if (p[i]!=nullptr){
				buf.push_back(i);
				p[i]->print_ch(buf);
				buf.pop_back();
			}
		}
	}

};

class Dree {
	Node* p[10];
	public:
		Dree(){
			for (unsigned i=0;i<10;++i) p[i] = nullptr;
		}
		~Dree(){
			for (unsigned i=0;i<10;++i){
				if (p[i]!=nullptr){
					delete p[i];
					p[i]=nullptr;
				}
			}
		}
		void add_number(vector<unsigned> n){
			if (p[n[0]]==nullptr){
				p[n[0]] = new Node();
			}
			Node* t = p[n[0]];
			for (unsigned i=1;i<n.size();++i){
				if (t->p[n[i]]==nullptr){
					t->p[n[i]] = new Node();
				}
				t = t->p[n[i]];
			}
			t->end = true;
		}
		void add_number(unsigned n){
			if (p[n%10]==nullptr){
				p[n%10] = new Node();
			}
			Node* t = p[n%10];
			for (n/=10;n>=10;n/=10){
				if (t->p[n%10]==nullptr){
					t->p[n%10] = new Node();
				}
				t = t->p[n%10];
			}
			if (t->p[n]==nullptr) t->p[n] = new Node();
			t = t->p[n];
			t->end = true;
		}
		void print_numbers(){
			vector<unsigned> buf;
			for (unsigned i=0;i<10;++i){
				if (p[i]!=nullptr){
					buf.push_back(i);
					p[i]->print_ch(buf);
					buf.pop_back();
				}
			}
		}
};

int main(){
	Dree tm;
	//vector<unsigned> n = {1,2,3,4,5};
	//tm.add_number(n);
	//n[2] = 4;
	//tm.add_number(n);
	tm.add_number(187676);
	tm.add_number(987676);
	tm.add_number(676);
	tm.print_numbers();
	return 0;
}
