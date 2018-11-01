#include <iostream>
#include <fstream>
#include <string>

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
	BOTH = 31	
};
inline constexpr bool
operator&(brack_bit x,brack_bit y){
	return static_cast<bool>(static_cast<unsigned>(x) & static_cast<unsigned>(y));
}
inline constexpr brack_bit
operator|(brack_bit x,brack_bit y){
	return static_cast<brack_bit>(static_cast<unsigned>(x) & static_cast<unsigned>(y));
}

char right_of(char c){
	switch (c){
		case '{': return '}';
		case '[': return ']';
		case '"': return '"';
	}
	return c;		
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
};

int main(){
	std::ifstream f;
	f.open("test_json.json",std::ifstream::in);	
	ChunkReader chk = ChunkReader(f,1024);
	char c;
	c = chk.untils(brack_bit::CURLYB);
	if (c=='\0') return 0;
	*chk;
	c = chk.untils( brack_bit::OPEN);
	*chk; // move along
	// skip until the { is closed with } or another brack is opened.
	std::string s;
	switch (c){
		case '"':
			// found a " so lets get the property name.
			
			s = chk.capture_untils(brack_bit::DOUBLEQ);
			break;
		case '}':
			// found a } so this is the end
			return 0;
			break;
		// its possible to find other opens.. but that would be malformed
	}
	if (c=='\0') return 0;
	*chk; // advance 1 char
	std::cout << "property name was " << s;
	// we don't know the property's type so, let's parse more.
	c = chk.untils(brack_bit::OPEN);
	if (c=='\0') return 0;
	*chk;
	switch (c){
		case '"':
			// it was a string. let's print it.
			s = chk.capture_untils(brack_bit::DOUBLEQ);
			// since we know its a string, we use DOUBLEQ instead of CLOSE.
			std::cout << " whose value was " << s << '\n';
			break;
		case '[':
			std::cout << " whose value was an array.\n";
			break;
		case '{':
			std::cout << " whose value was an object.\n";
			break;
	}
	f.close();
	return 0;
}
