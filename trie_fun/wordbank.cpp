/** wordbank.cpp has the function details for wordbank.
 * A wordbank (trie) in this case is stored as a tree of
 * nodes with an array of ptrs to 26 child nodes (the
 * alphabet).
 * Looks like I focused on allocation/deallocation.
 *
 */

#include <cstring>
#include "wordbank.h"

struct Node {
	Node* p[26];
	bool word_end;
	Node(): word_end(false){
		for (size_t i=0;i<26;++i) p[i]=nullptr;
	}
	~Node(){
		for (size_t i=0;i<26;++i){
			if (p[i]!=nullptr){
				std::cout<<(char)('a'+i)<<" released\n";
				delete p[i];
				p[i]=nullptr;
			}
		}
	}
	void add_child(const char c){
		if (p[c-97]==nullptr){
			p[c-97]=new Node();
			std::cout<<char(c)<<" allocated\n";
		}
	}
	bool has_child(const char c){
		return p[c-97]!=nullptr;
	}
	Node* child_c(const char c){
		return p[c-97];
	}
};
WordBank::WordBank(){
	for (size_t i=0;i<26;++i) p[i]=nullptr;
}
WordBank::~WordBank(){
	for (size_t i=0;i<26;++i){
		if (p[i]!=nullptr){
			delete p[i];
			std::cout<<(char)(i+97)<<" deleted\n";
			p[i]=nullptr;
		}
	}
}
void WordBank::add_word(const string& w){
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
}
void WordBank::add_word(const char* w){
	if (p[w[0]-97]==nullptr){
		p[w[0]-97]=new Node();
		std::cout<<w[0]<<" Made node\n";
	}
	Node* l = p[w[0]-97];
	unsigned letters=strlen(w);
	for (size_t i=1;i<letters;++i){
		l->add_child(w[i]);
		//if (l->p[w[i]-97]==nullptr){
			//l->p[w[i]-97]=new Node();
			//std::cout<<w[i]<<" Made node\n";
		//}
		l=l->p[w[i]-97];
	}
}
bool WordBank::prefix_exists(const string& s){
	Node* l = p[s[0]-97];
	if (l==nullptr) return 0;
	size_t i=1;
	//for (Node* l=p[s[i]-97];i<s.length() && l!=nullptr;l=l->p[s[i]-97]){
	//	++i;
	//}
	while (l!=nullptr && i<s.length()){
		l=l->p[s[i]-97];
		++i;
	}
	return i==s.length();
}
