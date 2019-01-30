#include "parser.h"
#include <iostream>
#include <stack>
#include <exception>
#include "../bmarksg/bmarksg.h"
#include "../chunkreader/ireaderfactory.h"

using std::cout;
using std::cerr;
using std::stack;

struct JsoNameGid {
	Jso* j;
	const int gid;
	string* name;
	JsoNameGid(Jso* obj, const int n, string* label):
		j(obj), gid(n), name(label) {}
};

bool has_key_string_value(Jso* j,const string& key,string** out){
	// if j is a JType::Obj and has the key then:
	// set out to point to it and return true.
	if (j->t==JType::Obj){
		Jso* v = j->key_value(key);
		if (v!=nullptr){
			*out = v->x.s;
			return true;
		}
	}
	return false;
}
bool has_property(Jso* j,const string& key){
	return (j->t==JType::Obj && j->key_value(key)!=nullptr);
}

void treeFromChunk(IReader* chr, JSON& tree){
	if (chr->empty()){
		throw std::invalid_argument("No input or non-existent file.");
	}
	if (nextNonWS(chr)!='{'){
		std::out_of_range("Could not find opening curly brace.");
	}
	chr->advance();
	parse_file_comma(chr,tree);
}

string* keyString(Jso* j){
	return ((j) ? j->Str() : nullptr);
}
ulong asULong(Jso* j){
	if (j){
		const char *s = j->Str()->c_str();
		if (s){
			return strtoul(s,NULL,0);
		}
	}
	return 0;
}

struct Bookmark {
	string *url, *title;
	string mtime;
	bool validFill(Jso* j){
		if (!j){
			return false;
		}
		title = keyString(j->key_value("title"));
		url = keyString(j->key_value("uri"));
		mtime = std::to_string(asULong(j->key_value("lastModified"))/1000000);
		// since it was stored in microseconds.
		return (title && url && mtime.length());
	}
	void insert2DB(DB_Connection &db, int gid){
		db.add_data(*title,*url,mtime,gid);
	}
};

void abridgedOut(ostream& out,const string& s,unsigned cnt){
	cnt = ((cnt<=s.length()) ? cnt : s.length()) ;
	for (unsigned i=0;i<cnt;++i){
		out << s[i];
	}
}

int main(int argc, char** argv){
	IReaderFactory reader_maker;
	IReader *textChunk;
	if (argc<2){
		cout << "No input file. Taking input from stdin.\n";
		string userInput;
		getline(std::cin,userInput);
		if (userInput.length()==0){
			cerr << "Nothing was entered.\nQuiting program.\n";
			return 1;
		}
		textChunk = reader_maker.ByInput(userInput);
	} else {
		cout << "Using file: " << argv[1] << '\n';
		textChunk = reader_maker.ByFile(argv[1],1024);
	}

	cout << "Begin Parse\n";
	JSON jsonTree;
	try {
		treeFromChunk(textChunk,jsonTree);
	}
	catch (const std::runtime_error& e){
		cerr << e.what()
			<< "\nStructure incomplete. Printing what was recieved:\n"
			<< jsonTree;
		return 1;
	}
	catch (const std::exception& e){
		cerr << e.what() << '\n';
		return 1;
	}
	cout << "End Parse\n";

	if (!(*jsonTree)->key_value("title")){
		cout << "****Imported data START****\n" << jsonTree << "\n****Imported data END****\n";
		return 0;
	}
	cout << "Begin Save\n";
	DB_Connection my_db;
	Bookmark link;
	stack<JsoNameGid> stk;
	// Create a db group to hold what's going to be inserted.
	stk.emplace((*jsonTree),my_db.create_group("group #1"),(*jsonTree)->key_value("title")->x.s);
	Jso* j;
	int pid, gid;
	string *s, *value;

	while (!stk.empty()){
		j = stk.top().j;
		pid = stk.top().gid;
		s = stk.top().name;
		stk.pop();
		// If top item was a 'x-moz-place-container', it should have an array named children.
		if (j->key_value("type")){
			j->key_value("type")->Get(&value);
			if ((*value)=="text/x-moz-place-container"){
				// So we create a group under the current parent.
				gid = my_db.create_group(*s,pid);
				if (gid==0){
					cerr << "Could not create a group.\n";
					return 1;
				}
				// And since its a group, we queue( stack) up its children.
				if (has_property(j,"children") && (*j)["children"]->t==JType::Arr){
					for (const auto& k : *((*j)["children"]->x.a) ){
						stk.emplace(k,gid,(*k)["title"]->x.s);
					}
					cout << "Made a group: " << *s << '\n';
				}
			} else if ((*value)=="text/x-moz-place"){
				// It must be a link then.
				if (link.validFill(j)){
					link.insert2DB(my_db,gid);
					cout << "Inserted link: ";
					abridgedOut(cout,*link.title,36);
					cout << '\n';
				}
			}
		}
	}
	cout << my_db.export_memory("/mnt/ramdisk/test.db") << '\n';
	return 0;
}
