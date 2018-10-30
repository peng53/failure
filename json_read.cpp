#include <iostream>
#include <fstream>
#include <string>

int main(){
	std::ifstream f;
	f.open("test.json",std::ifstream::in);
	char s[1024];
	std::string prop;
	prop.reserve(24);
	int next_is;
	while (f.good()){
		f.get(s,1024);
		for (size_t i=0;s[i]!='\0';++i){
			switch (s[i]){
				case '"':
					++i;
					prop.erase();
					while (i<1024 && s[i]!='"'){
						prop += s[i];
						++i;
					}
					std::cout << prop << '\n';
					break;
			}		
		}
	}
	f.close();
	return 0;
}
