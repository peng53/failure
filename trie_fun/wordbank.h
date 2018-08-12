/**
 * wordbank.h tries to implement a 'trie' for containing words
 * in a wordbank (as the filename suggests). See below for
 * possible member functions. See wordbank.cpp for implemation
 * details and main.cpp for usage.
 * Note: not every member function has been implemented. See
 * wordbank.cpp.
 */
#ifndef WORD_BNK
#define WORD_BNK

#include <vector>
#include <string>
#include <iostream>
using std::vector;
using std::string;
using std::ostream;

struct Node;
class WordBank {
	private:
		Node* root;
		Node* prefix(const char* s,size_t lc);
	public:
		WordBank();
		~WordBank();
		bool prefix_exists(const string& prefix,size_t lc);
		bool prefix_exists(const string& prefix);
		bool prefix_exists(const char* s,size_t lc);
		bool prefix_exists(const char* s);
		vector<char> next_possible_letters(string& prefix);

		vector<string> with_prefix(const string& prefix);

		void add_word(const char* w,size_t letters);
		void add_word(const char* w);

		void remove_word(const char* w,size_t lc);
		void remove_word(const char* w);
		void remove_word(const string& w,size_t lc);
		void remove_word(const string& w);

		ostream& import_words(ostream& o);
		int import_words(const vector<string>& o);
		ostream& export_words(ostream& o);
		vector<string> as_vector();
		int count_words();
		int with_prefix_count_word(const string& prefix);
		int with_prefix_remove_words(const string& prefix);
		bool operator[](const string &key);
		bool operator[](const char* s);
		
		void dump_levels(ostream &o);
};

#endif
