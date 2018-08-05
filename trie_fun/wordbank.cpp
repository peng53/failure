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
	~Node(){
		/*
		// Disabled currently.
		for (size_t i=0;i<26;++i){
			if (p[i]!=nullptr){
				std::cout<<(char)('a'+i)<<" released\n";
				delete p[i];
				p[i]=nullptr;
			}
		}
		*/
	}
	void add_child(const char c){
		if (p[cInd(c)]==nullptr){
			p[cInd(c)]=new Node();
			std::cout<<char(c)<<" allocated\n";
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
};
WordBank::WordBank(){
	for (size_t i=0;i<26;++i) p[i]=nullptr;
}
WordBank::~WordBank(){
	for (size_t i=0;i<26;++i){
		if (p[i]!=nullptr){
			//delete p[i];
			p[i]->delete_all();
			std::cout<<(char)(i+97)<<" deleted\n";
			//p[i]=nullptr;
		}
	}
}
void WordBank::add_word(const string& w){
	/*
	Node* l;
	if (p[w[0]-97]==nullptr){
		p[w[0]-97]=new Node();
		std::cout<<w[0]<<" Made node\n";
	}
	l = p[w[0]-97];
	for (size_t i=1;i<w.length();++i){
		l->add_child(w[i]);
		//if (l->p[w[i]-97]==nullptr){
			//l->p[w[i]-97]=new Node();
			//std::cout<<w[i]<<" Made node\n";
		//}
		l=l->p[w[i]-97];
	}
	l->word_end += 1;
	*/
	add_word(w.c_str());
}
void WordBank::add_word(const char* w,size_t letters){
	if (p[w[0]-97]==nullptr){
		p[w[0]-97]=new Node();
		std::cout<<w[0]<<" Made node\n";
	}
	Node* l = p[w[0]-97];
	//unsigned letters=strlen(w);
	for (size_t i=1;i<letters;++i){
		l->add_child(w[i]);
		//if (l->p[w[i]-97]==nullptr){
			//l->p[w[i]-97]=new Node();
			//std::cout<<w[i]<<" Made node\n";
		//}
		l=l->p[w[i]-97];
	}
	l->word_end += 1;
}
void WordBank::add_word(const char* w){
	add_word(w,strlen(w));
}
Node* WordBank::prefix(const char* s,size_t lc){
	/*
	Node* l = p[s[0]-97];
	if (l==nullptr) return 0;
	size_t i = 1;
	while (l!=nullptr && i<lc){
		l=l->p[s[i]-97];
		++i;
	}
	*/
	size_t i = 1;
	Node* n = p[cInd(s[0])];
	while (n!=nullptr && i<lc){
		n=n->p[cInd(s[i++])];
	}
	return (i==lc ? n : nullptr);
}
bool WordBank::prefix_exists(const char* s,size_t lc){
	return prefix(s,lc);
}
bool WordBank::prefix_exists(const char* s){
	return prefix_exists(s,strlen(s));
}
bool WordBank::prefix_exists(const string& s,size_t lc){
	return prefix(s.c_str(),lc);
}
bool WordBank::prefix_exists(const string& s){
	return prefix_exists(s,s.length());
}

bool WordBank::operator[](const char* s){
	Node* n = prefix(s,strlen(s));
	return (n!=nullptr && n->word_end);
}
bool WordBank::operator[](const string& s){
	return operator[](s.c_str());
}

/*bool WordBank::operator[](const string &s){
	//Node* l = p[s[0]-97];
	//size_t i=1;
	//while (l!=nullptr && i<s.length()){
	//	l=l->p[s[i++]-97];
	//}
	//return (i==s.length() && l->word_end);
	string::const_iterator c = s.cbegin();
	Node* l = p[cInd(*c)];
	while (l!=nullptr && c!=s.cend()){
		l=l->p[cInd(*c)];
		++c;
	}
	return (c==s.cend() && l->word_end);
}*/
/*
bool WordBank::operator[](const char* s){
	int l = strlen(s), i = 0;
	Node* n = p[cInd(s[0])];
	while (n!=nullptr && i<l){
		n=n->p[cInd(s[i])];
		++i;
	}
	return (i==l && n->word_end);
}
*/
/*
bool WordBank::prefix_exists(const string& s){
	Node* l = p[s[0]-97];
	if (l==nullptr) return 0;
	size_t i=1;
	while (l!=nullptr && i<s.length()){
		l=l->p[s[i]-97];
		++i;
	}
	//for (Node* l=p[s[i]-97];i<s.length() && l!=nullptr;l=l->p[s[i]-97]){
	//	++i;
	//}
	return i==s.length();
}
*/
vector<string> WordBank::as_vector(){
	queue<pair<string,Node*>> q;
	for (char i='a';i<='z';++i){
		if (p[i-97]!=nullptr){
			//q.push(pair<string,Node*>(string(1,i),p[i-97]));
			q.emplace(string(1,i),p[i-97]);
		}
	}
	Node* pp;
	string s;
	vector<string> R;
	while (!q.empty()){
		s = move(q.front().first);
		pp = q.front().second;
		q.pop();
		if (pp->word_end){
			R.emplace_back(s);
		}
		for (char i='a';i<='z';++i){
			if (pp->p[i-97]!=nullptr){
				//q.push(pair<string,Node*>(s+i,pp->p[i-97]));
				q.emplace(s+i,pp->p[i-97]);
			}
		}
	}
	
	return R;
}

void WordBank::remove_word(const char* s,size_t lc){
	Node* l = p[cInd(s[0])];
	if (l==nullptr) return;
	stack<Node*> stk;
	stk.push(l);
	size_t i=1;
	while (i<lc && l->p[cInd(s[i])]!=nullptr){
		l=l->p[cInd(s[i])];
		stk.push(l);
		std::cout << "pushed " << l << '\n';
		++i;
	}
	if (i!=lc || !(l->word_end)){
		// Word isn't in WordBank
		return;
	}
	l->word_end = 0;
	int j=lc-1;
	while (j>=0){
		l = stk.top();
		if (j+1!=lc){
			// Removes dangling ptr from last delete.
			// TODO make this more elegant.
			l->p[cInd(s[j+1])] = nullptr;
		}
		stk.pop();
		l->p[cInd(s[j])] = nullptr;
		// This child does not exist if this is the current node.
		if (l->word_end){
			return;
			// Means this node is needed.
		}
		for (size_t i=0;i<26;++i){
			if (l->p[i]!=nullptr){
				return;
				// This node and parent cannot be deleted. Done.
			}
		}
		std::cout << "1 deleted " << l << '\n';
		delete l;
		--j;
	}
	// Looks like entire word was deleted.
	p[cInd(s[0])] = nullptr;
}
void WordBank::remove_word(const char* s){
	remove_word(s,strlen(s));
}
void WordBank::remove_word(const string &s,size_t lc){
	remove_word(s.c_str(),lc);
}
void WordBank::remove_word(const string &s){
	remove_word(s.c_str(),s.length());
}
/*
void WordBank::remove_word(const string &s){
	//Node* l = p[s[0]-97];
	Node* l = p[cInd(s[0])];
	if (l==nullptr) return;
	std::cout << "pushed " << l << '\n';
	stack<Node*> stk;
	stk.push(l);
	size_t i=1;
	while (i<s.length() && l->p[cInd(s[i])]!=nullptr){
		l=l->p[cInd(s[i])];
		stk.push(l);
		std::cout << "pushed " << l << '\n';
		++i;
	}
	if (i!=s.length() || !(l->word_end)){
		// Word isn't in WordBank
		return;
	}
	l->word_end = 0;
	int j=s.length()-1;
	while (j>=0){
		l = stk.top();
		if (j+1!=s.length()){
			// Removes dangling ptr from last delete.
			// TODO make this more elegant.
			l->p[cInd(s[j+1])] = nullptr;
		}
		stk.pop();
		l->p[cInd(s[j])] = nullptr;
		// This child does not exist if this is the current node.
		if (l->word_end){
			return;
			// Means this node is needed.
		}
		for (size_t i=0;i<26;++i){
			if (l->p[i]!=nullptr){
				return;
				// This node and parent cannot be deleted. Done.
			}
		}
		std::cout << "1 deleted " << l << '\n';
		delete l;
		--j;
	}
	// Looks like entire word was deleted.
	p[cInd(s[0])] = nullptr;
}
*/