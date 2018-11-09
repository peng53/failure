#include <iostream>
#include <fstream>
#include <string>
#include <stack>

const size_t CHUNK = 1024;

enum class VType {STR, ARR, OBJ, DIG};

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
		ChunkReader(const ChunkReader &cr): ifs(cr.ifs),chunk_size(cr.chunk_size),start_str(0),cur_index(0){
			chars = new char[chunk_size];
			ifs.get(chars,chunk_size);
		}
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


enum class Prop {title, uri, children, junk, eof};

Prop get_next_prop(ChunkReader &cr){
	char c = cr.untils(brack_bit::DOUBLEQ | brack_bit::CURLYR);
	if (c=='}'){
		return Prop::eof;
	} else if (c=='"'){
		*cr;
		std::string s = cr.capture_untils(brack_bit::DOUBLEQ);
		std::cout << s << '\n';
		if (!cr.dead()){
			if (s.compare("title")==0) return Prop::title;
			else if (s.compare("uri")==0) return Prop::uri;
			else if (s.compare("children")==0) return Prop::children;
			return Prop::junk;
		}
	}
	return Prop::eof;
}
void skip_next(ChunkReader &cr){
	char c = cr.untils(brack_bit::OPEN | brack_bit::DIGIT);
	if (isdig(c)){
		cr.get_number();
	} else {
		
	}
}

int main(){
	std::ifstream f;
	f.open("test.json",std::ifstream::in);	
	ChunkReader chk = ChunkReader(f,1024);
	std::string title;
	int id = 0;
	std::stack<int> groups;
	char c;
	while (!chk.dead()){
		std::cout << groups.size() << '\n';
		if (groups.empty()){
			c = chk.untils(brack_bit::CURLYB);
			if (c!='{') return 0;
			*chk;
			groups.emplace(++id);
		} else {
			switch (get_next_prop(chk)){
				case Prop::junk:
					skip_next(chk);
					break;
				case Prop::eof:
					return 1;
					break;
				case Prop::uri:
					std::cout << "URI :" << chk.capture_untils(brack_bit::DOUBLEQ) << '\n';
					*chk;
					break;
				case Prop::title:
					*chk;
					title = chk.capture_untils(brack_bit::DOUBLEQ);
					std::cout << "Group #" << id << '\n';
					std::cout << "Title :" << title << '\n';
					*chk;
					break;
				case Prop::children:
					chk.untils(brack_bit::SQUAREB);
					chk.untils(brack_bit::CURLYB);
					*chk;
					groups.emplace(++id);
					break;
			}
		}
	}
	/*
	std::stack<Context> items;
	char c;
	c = chk.untils(brack_bit::CURLYB);
	if (c=='\0') return 0;
	*chk;
	items.emplace();
	std::string s;
	VType v;
	while (!items.empty()){
		// get a key. or end current context.
		c = chk.untils(brack_bit::BOTH);
		switch (c){
			case '"':
				// get start of key.
				*chk;
				s = chk.capture_untils(brack_bit::DOUBLEQ);
				std::cout << s;
				*chk;
				c = chk.untils(brack_bit::OPEN | brack_bit::CURLYR | brack_bit::DIGIT);
				switch (c){
					case '{':
						// an object..
						v = VType::OBJ;
						std::cout << "OBJ = ";
						items.emplace();						
						*chk;
						break;
					case '[':
						// an array..
						v = VType::ARR;
						std::cout << "ARR = ";
						items.emplace();
						*chk;
						break;
					case '"':
						// a string
						*chk;
						v = VType::STR;
						std::cout << "STR = ";
						//s = chk.capture_untils(brack_bit::DOUBLEQ);
						//std::cout << '=' << s << '\n';
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
						//s = chk.get_number();
						// don't advance otherwise the next symbol is discarded.
						//std::cout << '=' << s << '\n';
						v = VType::DIG;;
						std::cout << "DIG = ";
						break;
				}
				break;
			case '{':
			case '[':
				*chk;
				break;
			case '}':
				// found end of object.
				std::cout << items.top().title;
				std::cout << items.top().id;
				items.pop();
				break;
			case ']':
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
		switch (v){
			case VType::STR:
					s = chk.capture_untils(brack_bit::DOUBLEQ);
					*chk;
					std::cout << s << '\n';
					//items.top().title = s;

				break;
			case VType::DIG:
				s = chk.get_number();
				//don't advance otherwise the next symbol is discarded.
				std::cout <<  s << '\n';
				break;
			case VType::OBJ:
				break;
			case VType::ARR:
				break;
		}
	}
	*/
	f.close();
	return 0;
}
