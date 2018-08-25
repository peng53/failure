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
#include <ostream>

using std::vector;
using std::array;
using std::string;
using std::ostream;

static const size_t NSIZE = 36;

struct Node;
class WordBank {
	private:
		Node* root;
		//Node* prefix(const string& s,size_t lc) const;
		WordBank(Node*); // constructs a WordBank from a Node
	public:
		WordBank(); // constructs an empty WordBank
		WordBank(const WordBank& ws) noexcept; // constructs a WordBank from another
		~WordBank(); // destructor, destroys root, so root is more-or-less unique to this WordBank

		bool prefix_exists(const string& s,size_t lc=0) const; // returns whether prefix 's' exists
		vector<string> with_prefix(const string& s,size_t lc=0) const; // returns all words with prefix 's' from WordBank as vector of strings
		vector<string> words() const; // returns all words in WordBank as vector of strings
		bool operator[](const string &key) const; //check whether key exists as a word
		vector<char> next_possible_letters(const string& s,size_t lc=0) const; // returns chars that lead off of prefix 's'
		array<bool,NSIZE> next_tf(const string& s,size_t lc=0) const; // returns t/f array of cars that lead off of prefix 's'

		// int count_words();
		// int with_prefix_count_word(const string& prefix);
		// int with_prefix_remove_words(const string& prefix);

		friend ostream& operator<<(ostream& o,const WordBank& ws); // outputs words seperated by linefeed

		void new_word(const string& s,size_t letters=0); // adds word s[0:lc] to WordBank
		void remove_word(const string& w,size_t lc=0); // removes word 's' from WordBank, if it exists
		WordBank& operator<<(const string& s);  // adds a word. (new_word but, s[0:s.length()])
		WordBank operator=(const WordBank& rhs); // assignment operator, assigns rhs to this
		WordBank prefix_subset(const string &s,size_t lc=0); // returns a WordBank with only words that have prefix s[0:lc]
		WordBank& operator+=(const WordBank& rhs); // copies words from rhs to this
		WordBank operator+(const WordBank& rhs); 
};

#endif
