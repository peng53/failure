#include "parser.h"
#include <iostream>
#include <sstream>
#include <stack>
#include "../bmarksg/bmarksg.h"

using std::cout;
using std::cerr;
using std::istringstream;
using std::filebuf;
using std::ios_base;
using std::stack;
using std::pair;

struct JsoNameGid {
	Jso* j;
	const int gid;
	string* name;
	JsoNameGid(Jso* obj, const int n, string* label):
		j(obj), gid(n), name(label) {}
};

int main(int argc, char** argv){
	streambuf* f;
	istringstream ins_str;
	filebuf my_fb;
	if (argc<2){
		cout << "No input file. Taking input from stdin.\n";
		string userInput;
		getline(std::cin,userInput);
		if (userInput.length()==0){
			cout << "Nothing was entered.\nQuiting program.\n";
			return 1;
		}
		ins_str.str(userInput);
		f = ins_str.rdbuf();
	} else {
		cout << "Using file: "
			<< argv[1]
			<< '\n';
		my_fb.open(argv[1],ios_base::in);
		if (!my_fb.is_open()){
			cout << "Could not open file.\n";
			return 1;
		}
		f = &my_fb;
	}
	AReader textChunk(f,1024);
	if (next_symplex(textChunk)!='{'){
		cerr << "Could not find opening curly brace.\n";
		return 1;
	}
	textChunk.advance();
	JSON jsonTree;
	try {
		parse_file(textChunk,jsonTree);
	} catch (const std::runtime_error& e) {
		cerr << e.what()
			<< "\nStructure incomplete. Printing what was recieved:\n"
			<< jsonTree;
		return 1;
	}
	cout << jsonTree;
	if (my_fb.is_open()){
		my_fb.close();
	}
	DB_Connection my_db;
	stack<JsoNameGid> stk;
	// Create a db group to hold what's going to be inserted.
	stk.emplace((*jsonTree),my_db.create_group("group #1"),(*jsonTree)->key_value("title")->x.s);
	Jso* j;
	int pid, gid;
	string* s;
	while (!stk.empty()){
		j = stk.top().j;
		pid = stk.top().gid;
		s = stk.top().name;
		stk.pop();
		// If top item was a 'x-moz-place-container', it should have an array named children.
		if ((*(j->key_value("type")->x.s))=="text/x-moz-place-container"){
			// So we create a group under the current parent.
			gid = my_db.create_group(*s,pid);
			if (gid==0){
				cerr << "Could not create a group.\n";
				return 1;
			}
			// And since its a group, we queue( stack) up its children.
			for (const auto& k : *(j->key_value("children")->x.a) ){
				stk.emplace(k,gid,k->key_value("title")->x.s);
			}
			cout << "Made a group: " << *s << '\n';
		} else {
			// It must be a link then.
			// So we add the link.
			my_db.add_data(*s,*(j->key_value("uri"))->x.s,"",gid);
			cout << "Added data for: " << *s << '\n';
		}
	}
	cout << my_db.export_memory("test.db");
	return 0;
}
