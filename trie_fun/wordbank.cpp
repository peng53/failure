/** wordbank.cpp has the function details for wordbank.
 * A wordbank (trie) in this case is stored as a tree of
 * nodes with an array of ptrs to 26 child nodes (the
 * alphabet).
 * Looks like I focused on allocation/deallocation.
 *
 */

#include <queue>
#include <stack>
#include <utility>
#include <iostream>
#include "wordbank.h"

using std::queue;
using std::stack;
using std::pair;

size_t tr(char c){
	if (c>='0' && c<='9') return c-48;
	if (c>='A' && c<='Z') return c-55;
	if (c>='a' && c<='z') return c-87;
	return NSIZE; // invalid char
}
struct NSeq {
	size_t i;
	NSeq(): i(0){}
	char c(){
		if (i<=9) return '0'+i;
		return ((i<10) ? '0'+i : 'a'+(i-10));
	}
	char operator*(){
		return c();
	}
	char operator++(){
		++i;
		return c();
	}
	bool live(){
		return i<NSIZE;
	}
};

struct Node {
	Node* p[NSIZE];
	unsigned word_end; // tracks how many times a word has been added.
	Node(): word_end(0){
		// Creates a node with no children.
		for (size_t i=0;i<NSIZE;++i) p[i]=nullptr;
	}
	Node* at(const size_t i){
		return ((i>=NSIZE) ? nullptr : p[i]);
	}
	Node* operator[](const char c){
		return at(tr(c));
	}
	Node* operator[](const size_t i){
		// returns child node at index
		return at(i);
	}
	Node* child_node(const char c){
		// Child node ignore non alphas.
		// Creates a child-node as char c if it doesn't exists
		// Then returns ptr to that position c.
		size_t i = tr(c);
		if (i==NSIZE){
			return this;
		}
		if (p[i]==nullptr){
			p[i] = new Node();
		}
		return p[i];
	}
	Node* nchild(size_t i){
		// Creates child-node at position i and returns it.
		p[i] = new Node();
		return p[i];
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
			for (size_t i=0;i<NSIZE;++i){
				if ((*l)[i]){
					todel.push((*l)[i]);
				}
			}
			delete l;
		}
	}
	void copyrt(Node* R){
		// Copies R's nodes into this.
		Node *source, *dest;
		queue<pair<Node*,Node*>> q;
		q.emplace(this,R);
		while (!q.empty()){
			dest = q.front().first;
			source = q.front().second;
			q.pop();
			dest->word_end += source->word_end;
			for (NSeq c; c.live(); ++c){
				if ((*source)[*c]){
					q.emplace(dest->child_node(*c),(*source)[*c]);
				}
			}
		}
	}
	Node* prefix(const string& s,size_t lc){
		Node* l = this;
		for (size_t i=0;i<lc && l;++i){
			l = (*l)[s[i]];
		}
		return l;
	}
};
/*
void node_walk(Node*&l,size_t i){
	if (!((*l)[i])){
		l->nchild(i);
	}
	l = (*l)[i];
}
void node_walk(Node*&l,char c){
	node_walk(l,tr(c));
}
*/
WordBank::WordBank(): root(new Node){}

WordBank::WordBank(Node* n): root(n){}

WordBank::WordBank(const WordBank& rhs) noexcept  : root(new Node){
	root->copyrt(rhs.root);
}
WordBank& WordBank::operator<<(const string& s){
	new_word(s,s.length());
	return *this;
}

WordBank WordBank::operator=(const WordBank& rhs){
	if (this!=&rhs){
		root->deleteRoot();
		root = new Node;
		root->copyrt(rhs.root);
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
		//l = (*l)[s[i]];
		if (!(l = (*l)[s[i]])) return;
		stk.push(l);
	}
	l->word_end = 0;
	for (size_t c=lc; c>0; --c){
		l = stk.top();
		stk.pop();
		if (c!=lc){
			l->p[tr(s[c])] = nullptr;
		}
		if (l->word_end) return;
		for (size_t i=0;i<NSIZE;++i){
			if (!(*l)[i]) return;
		}
		delete l;
	}
	root->p[tr(s[0])] = nullptr;
}

vector<string> WordBank::with_prefix(const string &s,size_t lc) const {
	if (lc==0) lc=s.length();
	Node* l;
	if (!(l=root->prefix(s,lc))){
		// Prefix didn't exist.
		return vector<string>();
	}
	queue<pair<string,Node*>> q;
	for (NSeq c; c.live(); ++c){
		if ((*l)[*c]){
			q.emplace(string(1,*c),(*l)[*c]);
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
		for (NSeq i; i.live(); ++i){
			if ((*l)[*i]){
				q.emplace(sp+*i,(*l)[*i]);
			}
		}
	}
	return R;
}
WordBank WordBank::prefix_subset(const string &s,size_t lc){
	if (lc==0) lc=s.length();
	Node *l, *R, *r;
	// Get to part where copy begins
	l = root->prefix(s,lc);
	if (!l){
		std::cout << "nope";
		return WordBank();
	}
	R = new Node;
	r = R;
	/*
	// Create prefix nodes, except last letter
	for (size_t i=0;i<lc-1;++i){
		node_walk(r,s[i]);
	}
	// Set 'last' node to be 'recursive' copy of prefix
	//r->p[s[lc-1]-97]->copyrt(l);

	r->p[s[lc-1]]->copyrt(l);
	*/
	for (size_t i=0;i<lc;++i){
		r = r->child_node(s[i]);
	}
	r->copyrt(l);
	return WordBank(R);	
}

bool WordBank::operator[](const string& s) const {
	Node* n = root->prefix(s,s.length());
	return (n && n->word_end);
}

vector<char> WordBank::next_possible_letters(const string& s,size_t lc) const {
	if (lc==0) lc = s.length();
	Node* n = root->prefix(s,lc);
	if (!n){
		// Prefix didn't exist.
		return vector<char>();
	}
	vector<char> R;
	for (NSeq c; c.live(); ++c){
		if ((*n)[*c]){
			R.push_back(*c);
		}
	}
	return R;
}
array<bool,NSIZE> WordBank::next_tf(const string& s,size_t lc) const {
	if (lc==0) lc = s.length();
	array<bool,NSIZE> R;
	Node* n = root->prefix(s,lc);
	if (!n){
		R.fill(false);
		return R;
	}
	for (size_t i=0;i<NSIZE;++i){
		R[i] = ((*n)[i]);
	}
	return R;
}

bool WordBank::prefix_exists(const string& s,size_t lc) const {
	if (lc==0) lc=s.length();
	return root->prefix(s,lc);
}
vector<string> WordBank::words() const {
	return with_prefix("",0);
}

ostream& operator<<(ostream& o,const WordBank& ws){
	vector<string> R = ws.words();
	for (auto s : R){
		o << s << '\n';
	}
	return o;
}
WordBank& WordBank::operator+=(const WordBank& rhs){
	root->copyrt(rhs.root);
	return *this;
}
WordBank WordBank::operator+(const WordBank& rhs){
	// Init another WordBank as copy of lhs
	// and then copy rhs to it
	WordBank AB = *this;
	AB += rhs;
	return AB;
}
