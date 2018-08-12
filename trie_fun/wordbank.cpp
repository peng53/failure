/** wordbank.cpp has the function details for wordbank.
 * A wordbank (trie) in this case is stored as a tree of
 * nodes with an array of ptrs to 26 child nodes (the
 * alphabet).
 * Looks like I focused on allocation/deallocation.
 *
 */

#include <cstring>
#include <queue>
#include <stack>
#include <utility>
#include "wordbank.h"

using std::queue;
using std::stack;
using std::pair;

char cLow(char c){
	if (c>='a' && c<='z') return c;
	if (c>='A' && c<='Z') return c+32;
	return 'z'; // this return is an error
}
size_t cInd(char c){
	return cLow(c)-97;
}

struct Node {
	Node* p[26];
	unsigned word_end;
	Node(): word_end(false){
		for (size_t i=0;i<26;++i) p[i]=nullptr;
	}
	void add_child(const char c){
		if (p[cInd(c)]==nullptr){
			p[cInd(c)]=new Node();
			std::cout<<char(c)<<" allocated " << p[cInd(c)] << "\n";
		}
	}
	bool has_child(const char c){
		return p[cInd(c)]!=nullptr;
	}
	Node* child_c(const char c){
		return p[cInd(c)];
	}
	void delete_all(){
		// Deletes this node & and its children node
		// via a stack, not recursively.
		stack<Node*> todel;
		todel.push(this);
		Node* par;
		while (!todel.empty()){
			par = todel.top();
			todel.pop();
			for (size_t i=0;i<26;++i){
				if (par->p[i]!=nullptr){
					todel.push(par->p[i]);
				}
			}
			delete par;
		}
	}
	void deleteRoot(){
		stack<Node*> todel;
		todel.push(this);
		Node* l;
		while (!todel.empty()){
			l = todel.top();
			todel.pop();
			for (size_t i=0;i<26;++i){
				if (l->p[i]!=nullptr){
					todel.push(l->p[i]);
				}
			}
			delete l;
		}
	}
};
WordBank::WordBank(): root(new Node){}

WordBank::~WordBank(){
	root->deleteRoot();
}

void WordBank::add_word(const string& s,size_t len){
	Node* l = root;
	for (size_t i=0;i<len;++i){
		l->add_child(s[i]);
		l = l->p[cInd(s[i])];
	}
	l->word_end += 1;
}

void WordBank::remove_word(const string& s,size_t lc){
	Node* l = root;
	stack<Node*> stk;
	for (size_t i=0;i<lc;++i){
		if (l->p[cInd(s[i])]==nullptr) return;
		l = l->p[cInd(s[i])];
		stk.push(l);
	}
	l->word_end = 0;
	for (size_t c=lc; c>0; --c){
		l = stk.top();
		stk.pop();
		if (c!=lc){
			l->p[cInd(s[c])] = nullptr;
		}
		if (l->word_end) return;
		for (size_t i=0;i<26;++i){
			if (l->p[i]!=nullptr) return;
		}
		std::cout << "removing " << l << '\n';
		delete l;
	}
	root->p[cInd(s[0])] = nullptr;
}

Node* WordBank::prefix(const string& s,size_t lc){
	Node* l = root;
	for (size_t i=0;i<lc;++i){
		if (l->p[cInd(s[i])]==nullptr) return nullptr;
		l = l->p[cInd(s[i])];
	}
	return l;
}

vector<string> WordBank::with_prefix(const string &s,size_t lc){
	Node* l = prefix(s,lc);
	if (l==nullptr){
		// Prefix didn't exist.
		return vector<string>();
	}
	queue<pair<string,Node*>> q;
	for (char i='a';i<='z';++i){
		if (l->p[i-97]!=nullptr){
			q.emplace(string(1,i),l->p[i-97]);
		}
	}
	vector<string> R;
	string sp;
	while (!q.empty()){
		sp = move(q.front().first);
		l = q.front().second;
		q.pop();
		if (l->word_end){
			R.emplace_back(sp);
		}
		for (char i='a';i<='z';++i){
			if (l->p[i-97]!=nullptr){
				q.emplace(sp+i,l->p[i-97]);
			}
		}
	}
	return R;
}

bool WordBank::operator[](const char* s){
	Node* n = prefix(s,strlen(s));
	return (n!=nullptr && n->word_end);
}
bool WordBank::operator[](const string& s){
	return operator[](s.c_str());
}
