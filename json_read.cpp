#include <iostream>
#include <fstream>
#include <string>

const size_t CHUNK = 1024;
enum NextString {TITLE, URI, CHILDREN, MORE, JUNK, DUNNO};

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
		std::string capture_untils(bool square,bool double_q,bool q,bool curly,bool parenth){
			std::string s;
			start_capture();
			while (!dead()){
				if (
					(square && chars[cur_index]==']') 
					|| (double_q && chars[cur_index]=='"') 
					|| (q && chars[cur_index]=='\'') 
					|| (curly && chars[cur_index]=='}') 
					|| (parenth && chars[cur_index]==')')
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
};

int main(){
	std::ifstream f;
	f.open("test.json",std::ifstream::in);	
	ChunkReader chk = ChunkReader(f,1024);
	chk.until('"'); // current sym is " or EOF
	*chk; // advance 1 char
	chk.start_capture(); // set anchor point.
	chk.until('"'); // skip to next "
	std::string s = chk.get_capture(); // get substr from anchor point to current index
	std::cout << s << '\n';
	chk.until('"'); // skip to next " .. or EOF
	*chk; // advance 1
	chk.until('"'); // skip to next " .. or EOF
	*chk; // advance 1
	s = chk.capture_until('"'); // get substr from this point until before next "
	*chk;
	std::cout << s << '\n';
	chk.until('"');
	*chk;
	s = chk.capture_untils(0,1,0,0,0);
	std::cout << s << '\n';
	f.close();

	return 0;
}
