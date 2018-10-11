// Count word prog

#include <iostream>
#include <fstream>
#include "wordbank.h"

using std::cout;

bool is_letter(char c){
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int main(int argc, char** argv){
	// if (argc<2){
		// return 1;
	// }
	WordBank ws, ws2, ws3;
	ws << "69cat" << "bat123" << "mat";
	ws.new_word_z("jacketcatbagmagoot",5);
	ws2 << "frogmanzoo456" << "telegraph" << "alphastrike" << "catwarriorz";
	ws3 << "zebrararitytango" << "poop";
	std::cout << "\nWordBank::ws\n" << ws << "\nWordBank::ws2\n" << ws2 << "\nWordBank::ws3\n" << ws3;
	ws += (ws2+ws3);
	std::cout << "\nWordBank::ws\n" << ws << "\nWordBank::ws2\n" << ws2 << "\nWordBank::ws3\n" << ws3;
	cout << ws.prefix_exists("69") << "? 69 is prefix\n";
	cout << ws.prefix_subset("69");
	vector<string> vs = ws.words();
	cout << "words in ws\n";
	for (auto s : vs){
		cout << s << '\n';
	}
	vs = ws.with_prefix("fro");
	cout << "words in ws with prefix fro\n";
	for (auto s : vs){
		cout << s << '\n';
		ws.remove_word("fro"+s);
	}
	vs = ws.with_prefix("fro");
	cout << "words in ws with prefix fro after removal\n";
	for (auto s : vs){
		cout << s << '\n';
	}
	cout << ws["mat"] << "? mat in ws\n";
	vector<char> vc = ws.next_possible_letters("fro");
	cout << "letter guess for 'fro'\n";
	for (auto s : vc){
		cout << s << '\n';
	}

	cout << "\n\n/* input test */\n\n";
	std::ifstream f;
	string line;
	f.open("words.txt");
	//int count = 0;
	while (getline(f,line)){// && count<8000){
		ws << line;
		//++count;
	}
	f.close();
	string my_prefix;
	int wc;
	do {
		cout << "Type a prefix, or ! to quit: ";
		std::cin >> my_prefix;
		if (my_prefix[0]=='!'){ break; }
		vs = ws.with_prefix(my_prefix);
		for (size_t i=((vs.size()>=50) ? 50 : vs.size()); i>0; --i){
			cout << my_prefix << vs[i-1] << '\n';
		}
	} while (my_prefix!="!");
	return 0;
}
