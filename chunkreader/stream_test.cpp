#include "stream_test.h"

AReader::AReader(const string& input):
	chars(input),
	i(chars.begin())
{}

AReader& AReader::operator++(){
	advance();
	return *this;
}
void AReader::advance(){
	if (i<chars.end()){
		++i;
	} else {
		good = false;
	}
}

void AReader::feed(){
	i = chars.end();
}
char AReader::get(){
	return (*i);
}
bool AReader::has_data(){
	return i<chars.end();
}
char AReader::until(char c,string* str_ptr){
	do {
		if (get()==c){
			return get();
		}
		if (str_ptr){
			(*str_ptr) += get();
		}
		++(*this);
	} while (has_data() || good);
	return '\0'; // this means no more data left.
}
bool AReader::empty(){
	return !(has_data() || good);
}

