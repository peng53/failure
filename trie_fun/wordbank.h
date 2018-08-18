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
#include <array>
#include <iostream>
using std::vector;
using std::array;
using std::string;
using std::ostream;

struct Node;
class WordBank {
	private:
		Node* root;
		Node* prefix(const string& s,size_t lc);
	public:
		WordBank();
		WordBank(Node*);
		WordBank(const WordBank& ws) noexcept;
		~WordBank();

		void new_word(const string& s,size_t letters=0);
		void remove_word(const string& w,size_t lc=0);
		bool prefix_exists(const string& s,size_t lc=0);
		vector<string> with_prefix(const string& s,size_t lc=0);

		vector<string> words();
		bool operator[](const string &key);
		vector<char> next_possible_letters(const string& s,size_t lc=0);
		array<bool,26> next_tf(const string& s,size_t lc=0);

		// ostream& import_words(ostream& o);
		// int import_words(const vector<string>& o);
		// ostream& export_words(ostream& o);
		// int count_words();
		// int with_prefix_count_word(const string& prefix);
		// int with_prefix_remove_words(const string& prefix);

		friend ostream& operator<<(ostream& o,WordBank& ws);
		// void dump_levels(ostream &o);
		WordBank& operator+=(const string& s);
		WordBank operator=(const WordBank& rhs);
		WordBank prefix_subset(const string &s,size_t lc=0);
};

#endif
