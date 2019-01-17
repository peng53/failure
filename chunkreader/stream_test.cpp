#include "stream_test.h"

void AReader::load(const string& input){
	chars = input;
	i = chars.begin();
	good = true;
}

AReader& AReader::operator++(){
	advance();
	return *this;
}
void AReader::advance(){
	if (good && i<chars.end()){
		++i;
	} else {
		good = false;
	}
}

void AReader::feed(){
	good = false;
}
char AReader::get(){
	return (good ? (*i) : '\0');
}
char AReader::until(char c,string* str_ptr){
	while (!empty()){
		if (get()==c){
			return get();
		}
		if (str_ptr){
			(*str_ptr) += get();
		}
		++(*this);
	}
	return '\0'; // this means no more data left.
}
bool AReader::empty(){
	return !(good && i<chars.end());
}

