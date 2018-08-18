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
#define ERR_CHAR '{'

using std::queue;
using std::stack;
using std::pair;

char cLow(char c){
	if (c>='a' && c<='z') return c;
	if (c>='A' && c<='Z') return c+32;
	return ERR_CHAR; // this return is an error
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
	Node* operator[](const char c){
		if (c>='a' && c<='z') return p[c-97];
		if (c>='A' && c<='Z') return p[c-65];
		return nullptr;
	}
	Node* operator[](const size_t i){
		return ((i>25) ? nullptr : p[i]);
	}

	void add_child(const char c){
		if (p[cInd(c)]==nullptr){
			p[cInd(c)]=new Node();
			std::cout<<c<<" allocated " << p[cInd(c)] << "\n";
		}
	}
	Node* child_node(const char c){
		// Child node ignore non alphas.
		size_t i = cInd(c);
		if (i>25){
			return this;
		}
		if (p[i]==nullptr){
			p[i] = new Node();
		}
		return p[i];
	}
	Node* nchild(size_t i){
		p[i] = new Node();
		return p[i];
	}
	bool has_child(const char c){
		return p[cInd(c)]!=nullptr;
	}
	Node* child_c(const char c){
		return p[cInd(c)];
	}
	void deleteRoot(){
		// Deletes this node & and its children node
		// via a stack, not recursively.
		stack<Node*> todel;
		todel.push(this);
		Node* l;
		while (!todel.empty()){
			l = todel.top();
			todel.pop();
			for (size_t i=0;i<26;++i){
				if ((*l)[i]){
					todel.push((*l)[i]);
				}
			}
			delete l;
		}
	}
	Node* copyr(){	
		Node *dest, *source;
		Node *R = new Node;
		queue<pair<Node*,Node*>> q;
		q.emplace(R,this);
		while (!q.empty()){
			dest = q.front().first;
			source = q.front().second;
			q.pop();
			dest->word_end += source->word_end;
			for (char c='a';c<='z';++c){
				if ((*source)[c]){
					q.emplace(dest->child_node(c),(*source)[c]);
				}
			}
		}
		return R;
	}
};
WordBank::WordBank(): root(new Node){}

WordBank::WordBank(Node* n): root(n){}

WordBank::WordBank(const WordBank& rhs) noexcept {
	root = (rhs.root)->copyr();
}
WordBank& WordBank::operator+=(const string& s){
	new_word(s,s.length());
	return *this;
}

WordBank WordBank::operator=(const WordBank& rhs){
	if (this!=&rhs){
		root->deleteRoot();
		root = (rhs.root)->copyr();		
	}
	return *this;
}

WordBank::~WordBank(){
	root->deleteRoot();
}

void WordBank::new_word(const string& s,size_t len){
	// Uses child_node which ignores non-alphas
	// and does the checking
	if (len==0) len=s.length();
	Node* l = root;
	for (size_t i=0;i<len;++i){
		l = l->child_node(s[i]);
	}
	l->word_end += 1;
}
void WordBank::remove_word(const string& s,size_t lc){
	if (lc==0) lc=s.length();
	Node* l = root;
	stack<Node*> stk;
	for (size_t i=0;i<lc;++i){
		l = (*l)[s[i]];
		if (!l) return;
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
			if (!(*l)[i]) return;
		}
		std::cout << "removing " << l << '\n';
		delete l;
	}
	root->p[cInd(s[0])] = nullptr;
}

Node* WordBank::prefix(const string& s,size_t lc){
	Node* l = root;
	for (size_t i=0;i<lc && l;++i){
		l = (*l)[s[i]];
	}
	return l;
}

vector<string> WordBank::with_prefix(const string &s,size_t lc){
	if (lc==0) lc=s.length();
	Node* l = prefix(s,lc);
	if (!l){
		// Prefix didn't exist.
		return vector<string>();
	}
	queue<pair<string,Node*>> q;
	for (char i='a';i<='z';++i){
		if ((*l)[i]){
			q.emplace(string(1,i),(*l)[i]);
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
			if ((*l)[i]){
				q.emplace(sp+i,l->p[i-97]);
			}
		}
	}
	return R;
}
WordBank WordBank::prefix_subset(const string &s,size_t lc){
	if (lc==0) lc=s.length();
	Node* l = prefix(s,lc);
	// Get to part where copy begins
	if (!l){
		std::cout << "no prefix";
		return WordBank();
	}
	Node* R = new Node;
	Node* r = R;
	// Create prefix nodes, except last letter
	for (size_t i=0;i<lc;++i){
		r = r->nchild(s[i]);
	}
	// Set 'last' node to be 'recursive' copy of prefix
	r->p[s[lc-1]] = l->copyr();
	return WordBank(R);	
}

bool WordBank::operator[](const string& s){
	Node* n = prefix(s,s.length());
	return (n && n->word_end);
}

vector<char> WordBank::next_possible_letters(const string& s,size_t lc){
	if (lc==0) lc = s.length();
	Node* n = prefix(s,lc);
	if (!n){
		// Prefix didn't exist.
		return vector<char>();
	}
	vector<char> R;
	for (char i='a';i<='z';++i){
		if ((*n)[i]){
			R.push_back(i);
		}
	}
	return R;
}
array<bool,26> WordBank::next_tf(const string& s,size_t lc){
	if (lc==0) lc = s.length();
	array<bool,26> R;
	Node* n = prefix(s,lc);
	if (!n){
		R.fill(false);
		return R;
	}
	for (size_t i=0;i<26;++i){
		R[i] = ((*n)[i]);
	}
	return R;
}

bool WordBank::prefix_exists(const string& s,size_t lc){
	if (lc==0) lc=s.length();
	return prefix(s,lc);
}
vector<string> WordBank::words(){
	return with_prefix("",0);
}

ostream& operator<<(ostream& o,WordBank& ws){
	vector<string> R = ws.words();
	for (auto s : R){
		o << s << '\n';
	}
	return o;
}
