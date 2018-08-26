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
struct CSeq {
	/* CSeq takes a reference of string and a length and will return
		a char when dereferenced.
	Yes, I am aware this is might be slightly overengineered, but
		I wanted to see if it would work.
	*/
	const string& s;
	size_t i, e;
	CSeq(const string& _s, size_t end): s(_s), i(0), e(end){}
	char operator*(){
		return s[i];
	}
	char operator++(){ // prefix only please!
		return s[++i];
	}
	bool live(){
		return i<e;
	}
	char operator--(){ // prefix only please!
		return s[--i];
	}
	bool evil(){
		return i>0;
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
	Node* childc(const char c){
		// Child node ignore non alphas.
		// Creates a child-node as char c if it doesn't exists
		// Then returns ptr to that position c.
		return childi(tr(c));
	}
	Node* childi(size_t i){
		// Creates child-node at position i and returns it.
		if (i>=NSIZE) return this;
		if (p[i]==nullptr) p[i] = new Node;
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
			/*
			for (NSeq c; c.live(); ++c){
				if ((*source)[*c]){
					q.emplace(dest->childc(*c),(*source)[*c]);
				}
			}
			*/
			for (size_t i=0;i<NSIZE;++i){
				if ((*source)[i]){
					q.emplace(dest->childi(i),(*source)[i]);
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
	Node* prefix(CSeq S){
		Node* l = this;
		for (; S.live(); ++S){
			l = (*l)[*S];
		}
		return l;
	}
};

WordBank::WordBank(): root(new Node){}

WordBank::WordBank(Node* n): root(n){}

WordBank::WordBank(const WordBank& rhs) noexcept  : root(new Node){
	root->copyrt(rhs.root);
}
WordBank& WordBank::operator<<(const string& s){
	new_word_z(s,s.length());
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
	if (len==0) return;
	Node* l = root;
	for (size_t i=0;i<len;++i){
		l = l->childc(s[i]);
	}
	l->word_end += 1;
}
void WordBank::new_word_z(const string& s,size_t len){
	if (len==0) len=s.length();
	if (len==0) return;
	Node* l = root;
	for (CSeq S(s,len); S.live(); ++S){
		l = l->childc(*S);
	}
	l->word_end += 1;
}
void WordBank::remove_word(const string& s,size_t lc){
	if (lc==0) lc=s.length();
	if (lc==0) return;
	Node* l = root;
	stack<Node*> stk;
	for (size_t i=0;i<lc;++i){
		//l = (*l)[s[i]];
		if (!(l = (*l)[s[i]])) return;
		stk.push(l);
	}
	l->word_end = 0;
	for (size_t c=lc; c>0; --c){
		// Wait a minute, there shouldn't be char at s[lc]!
		// Does this need fixing?
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
	//root->p[s[0]] = nullptr;
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
	r = R = new Node;
	// Create prefix nodes, except last letter
	// Set 'last' node to be 'recursive' copy of prefix
	for (size_t i=0;i<lc;++i){
		r = r->childc(s[i]);
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
	return with_prefix("");
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
void WordBank::with_prefix_remove_words(const string& s,size_t lc){
	if (lc==0) lc = s.length();
	Node* l = root->prefix(s,lc);
	if (!l) return;
	for (size_t i=0;i<NSIZE;++i){
		if ((*l)[i]){
			(*l)[i]->deleteRoot();
		}
	}
}
unsigned WordBank::with_prefix_count_words(const string& s,size_t lc){
	if (lc==0) lc = s.length();
	Node* l = root->prefix(s,lc);
	if (!l) return 0;
	unsigned c = 0;
	stack<Node*> stk;
	stk.push(l);
	while (!stk.empty()){
		l = stk.top();
		if (l->word_end) ++c;
		stk.pop();
		for (size_t i=0;i<NSIZE;++i){
			if ((*l)[i]){
				stk.push((*l)[i]);
			}
		}
	}
	return c;
}
unsigned WordBank::count_words(){
	return with_prefix_count_words("");
}