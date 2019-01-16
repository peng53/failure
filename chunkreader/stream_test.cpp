#include "stream_test.h"

AReader::AReader(const string& input):
	IReader(input.length()),
	chars(input),
	i(0)
{}

AReader& AReader::operator++(){
	advance();
	return *this;
}
void AReader::advance(){
	if (i<M){
		++i;
	} else {
		good = false;
	}
}

void AReader::feed(){
	i = M;
}
char AReader::get(){
	return chars[i];
}
bool AReader::has_data(){
	return i<M;
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

