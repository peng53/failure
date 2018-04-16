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
using std::ofstream;

struct Node;

class WordBank {
	private:
		Node* p[26];
	public:
		WordBank();
		~WordBank();
		bool prefix_exists(const string& prefix);
		vector<char> next_possible_letters(string& prefix);
		vector<string> with_prefix(string& prefix);
		void add_word(const string& w);
		void add_word(const char* w);
		void remove_word(const string& w);
		int import_words(ofstream& o);
		int import_words(const vector<string>& o);
		int export_words(ofstream& o);
		vector<string> as_vector();
		int count_words();
		int with_prefix_count_words(const string& prefix);
		int with_prefix_remove_words(const string& prefix);
};

#endif
