#include <iostream>
#include <fstream>
#include <string>
#include <stack>

const size_t CHUNK = 1024;
enum NextString {TITLE, URI, CHILDREN, MORE, JUNK, DUNNO};

enum class brack_bit : unsigned {
	DOUBLEQ = 1,
	CURLYB = 2,
	SQUAREB = 4,
	OPEN = 7,
	CURLYR = 8,
	SQUARER = 16,
	CLOSE = 25,
	BOTH = 31,
	DIGIT = 32
};
inline constexpr bool
operator&(brack_bit x,brack_bit y){
	return static_cast<bool>(static_cast<unsigned>(x) & static_cast<unsigned>(y));
}
inline constexpr brack_bit
operator|(brack_bit x,brack_bit y){
	return static_cast<brack_bit>(static_cast<unsigned>(x) | static_cast<unsigned>(y));
}

char right_of(char c){
	switch (c){
		case '{': return '}';
		case '[': return ']';
		case '"': return '"';
	}
	return c;		
}

bool isdig(char c){
	return ((c=='.') || (c>='0' && c<='9'));
}

class ChunkReader {
	private:
		std::ifstream& ifs;
		// Note to self, ifs has to be a reference since
		// ifstream cannot be owned?
		// otherwise the constructor would be ill-formed.
		size_t chunk_size;
		size_t start_str;
		size_t cur_index;
		char* chars;
	public:
		ChunkReader(std::ifstream& f,size_t ccnt):ifs(f), chunk_size(ccnt),start_str(0), cur_index(0){
			chars = new char[chunk_size];
			ifs.get(chars,chunk_size);
		}
		~ChunkReader(){
			delete[] chars;
		}
		bool dead(){
			return ifs.eof();
		}
		void feed(){
			ifs.get(chars,chunk_size);
			cur_index = 0;
		}
		void start_capture(){
			start_str = cur_index;
		}
		std::string get_capture(){
			return std::string(chars+start_str,chars+cur_index);
		}
		std::string& get_capture(std::string& s){
			s += get_capture();
			return s;
		}
		void until(char c){
			// note: should not use if characters skipped have value as the capture
			// # number is meaningless since this function can use feed.
			while (!dead()){
				if (chars[cur_index]==c){
					break;
				}
				++cur_index;
				if (cur_index==chunk_size){
					feed();
				}
			}
		}
		char untils(brack_bit bracks){
			// the bits of bracks represent if the char is included.
			// order is [{", with " being the first bit.
			while (!dead()){
				if (
				((bracks & brack_bit::DOUBLEQ) && (chars[cur_index]=='"'))
				|| ((bracks & brack_bit::CURLYB) && (chars[cur_index]=='{'))
				|| ((bracks & brack_bit::SQUAREB) && (chars[cur_index]=='['))
				|| ((bracks & brack_bit::CURLYR) && (chars[cur_index]=='}'))
				|| ((bracks & brack_bit::SQUARER) && (chars[cur_index]==']'))
				|| ((bracks & brack_bit::DIGIT) && isdig(chars[cur_index]))
				){
					break;
				}
				if (cur_index+1==chunk_size){
					feed();
				} else {
					++cur_index;
				}
			}
			return chars[cur_index];		
		}
		std::string capture_until(char c){
			// starts at cur_index on current feed.
			// will use feed while caching the results.
			std::string s;
			start_capture();
			while (!dead()){
				if (chars[cur_index]==c){
					// DONE!
					break;
				}
				if (cur_index+1==chunk_size){
					get_capture(s);
					start_capture();
					feed();
				} else {
					++cur_index;
				}
			}
			return get_capture(s);
		}
		std::string capture_untils(brack_bit bracks){
			std::string s;
			start_capture();
			while (!dead()){
				if (
				((bracks & brack_bit::DOUBLEQ) && (chars[cur_index]=='"'))
				|| ((bracks & brack_bit::CURLYB) && (chars[cur_index]=='}'))
				|| ((bracks & brack_bit::SQUAREB) && (chars[cur_index]==']'))
				){
					break;
				}
				if (cur_index+1==chunk_size){
					get_capture(s);
					start_capture();
					feed();
				} else {
					++cur_index;
				}
			}
			return get_capture(s);			
		}
		char operator*(){
			if (cur_index==chunk_size){
				feed();
			}
			char c = chars[cur_index];
			++cur_index;
			return c;
		}
		std::string get_quotedstring(){
			// returns all info until a double quote is encountered or EOF
			// might delete this one.
			return capture_untils(brack_bit::DOUBLEQ);
		}
		std::string get_number(){
			// current position is a digit.
			std::string s;
			start_capture();
			while (!dead()){
				if (!isdig(chars[cur_index])){
					break;
				}
				if (cur_index+1==chunk_size){
					get_capture(s);
					start_capture();
					feed();
				} else {
					++cur_index;
				}
			}
			get_capture(s);
			if (!isdig(s.back())){
				s.pop_back();
			}
			return s;
		}
};

enum class JType {group, place};

struct Context {
	std::string title;
	std::string uri;
	JType type;
	int id;	
};


int main(){
	std::stack<Context> items;
	Context current;
	std::ifstream f;
	f.open("test.json",std::ifstream::in);	
	ChunkReader chk = ChunkReader(f,1024);
	char c;
	c = chk.untils(brack_bit::CURLYB);
	if (c=='\0') return 0;
	*chk;
	items.push(current);
	std::string s;
	while (!items.empty()){
		// get a key. or end current context.
		c = chk.untils(brack_bit::DOUBLEQ | brack_bit::CURLYR);
		switch (c){
			case '"':
				// get start of key.
				*chk;
				s = chk.capture_untils(brack_bit::DOUBLEQ);
				std::cout << s;
				*chk;
				c = chk.untils(brack_bit::OPEN | brack_bit::CURLYR | brack_bit::DIGIT);
				switch (c){
					case '}':
						// no value?
						return 0;
						break;
					case '{':
						// an object..
						// ignore for now.
						std::cout << '\n';
						chk.untils(brack_bit::CURLYB);
						*chk;
						break;
					case '[':
						// an array..
						// ignore for now.
						std::cout << '\n';
						chk.untils(brack_bit::SQUARER);
						*chk;
						break;
					case '"':
						// a string
						*chk;
						s = chk.capture_untils(brack_bit::DOUBLEQ);
						*chk;
						std::cout << '=' << s << '\n';
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': // must be a digit.
						// don't advance otherwise the first digit is discarded.
						s = chk.get_number();
						// don't advance otherwise the next symbol is discarded.
						std::cout << '=' << s << '\n';
						break;
				}
				break;
			case '}':
				// found end of object.
				std::cout << items.top().title;
				std::cout << items.top().id;
				items.pop();
				break;
			default:
				// key not found.
				return 0;
				break;
		}
	}
	f.close();
	return 0;
}
