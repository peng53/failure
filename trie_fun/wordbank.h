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
		~WordBank();

		void new_word(const string& s,size_t letters);
		void new_word(const string& s){
			new_word(s,s.length());
		}
		void add_word(const string& s,size_t letters);
		void add_word(const string& s){
			add_word(s,s.length());
		}

		void remove_word(const string& w,size_t lc);
		void remove_word(const string& w){
			remove_word(w,w.length());
		}

		bool prefix_exists(const string& s,size_t lc){
			return prefix(s,lc);
		}
		bool prefix_exists(const string& s){
			return prefix_exists(s,s.length());
		}

		vector<string> with_prefix(const string& s,size_t lc);
		vector<string> with_prefix(const string& s){
			return with_prefix(s,s.length());
		}

		bool operator[](const string &key);

		vector<char> next_possible_letters(const string& s,size_t lc);
		vector<char> next_possible_letters(const string& s){
			return next_possible_letters(s,s.length());
		}
		array<bool,26> next_tf(const string& s,size_t lc);
		array<bool,26> next_tf(const string& s){
			return next_tf(s,s.length());
		}

		ostream& import_words(ostream& o);
		int import_words(const vector<string>& o);
		ostream& export_words(ostream& o);
		vector<string> as_vector();
		int count_words();
		int with_prefix_count_word(const string& prefix);
		int with_prefix_remove_words(const string& prefix);


		void dump_levels(ostream &o);
};

#endif
