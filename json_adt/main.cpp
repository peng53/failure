#include "parser.h"
#include "../bmarksg/bmarksg.h"
#include "../chunkreader/ireaderfactory.h"
#include <iostream>
#include <stack>
#include <exception>

using std::cout;
using std::cerr;
using std::stack;

static void treeFromChunk(IReader* chr, JSON& tree){
	if (chr->empty()){
		throw std::invalid_argument("No input or non-existent file.");
	}
	if (nextNonWS(chr)!='{'){
		std::out_of_range("Could not find opening curly brace.");
	}
	chr->advance();
	parse_file_comma(chr,tree);
}

static string* keyString(Jso* j){
	return ((j) ? j->Str() : nullptr);
}
static ulong asULong(Jso* j){
	if (j){
		const char *s = j->Str()->c_str();
		if (s){
			return strtoul(s,NULL,0);
		}
	}
	return 0;
}

static void abridgedOut(ostream& out,const string& s,unsigned cnt){
	cnt = ((cnt<=s.length()) ? cnt : s.length()) ;
	for (unsigned i=0;i<cnt;++i){
		out << s[i];
	}
}

enum class DBE { Group, Link, Invalid};

struct DB_Entry {
	string *title, *url;
	string mtime;
	DBE classify(Jso* j){
		DBE r = DBE::Invalid;
		if (!j || !keyString(j->key_value("type"))){
			// do nothing.
		} else if (*(keyString(j->key_value("type")))=="text/x-moz-place-container"){
			if (j->key_value("children")){
				r = DBE::Group;
			}
		} else if (*(keyString(j->key_value("type")))=="text/x-moz-place"){
			r = DBE::Link;
		}
		if (r!=DBE::Invalid){
			title = keyString(j->key_value("title"));
		}
		return r;
	}
	bool linkValid(Jso* j){
		url = keyString(j->key_value("uri"));
		mtime = std::to_string(asULong(j->key_value("lastModified"))/1000000);
		// since it was stored in microseconds.
		return (title && url && mtime.length());
	}
	void linkInsert(DB_Connection &db, int gid){
		db.add_data(*title,*url,mtime,gid);
	}
	int groupInsert(DB_Connection &db, int gid){
		return db.create_group(*title,gid);
	}
};

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
	DB_Entry entry;
	stack<pair<Jso*,int>> stk;
	// Create a db group to hold what's going to be inserted.
	int pid;
	int gid = my_db.create_group("group #1");
	stk.emplace(*jsonTree,gid);
	Jso* j;

	while (!stk.empty()){
		j = stk.top().first;
		pid = stk.top().second;
		stk.pop();
		switch (entry.classify(j)){
			case DBE::Group:
				gid = entry.groupInsert(my_db,pid);
				if (gid==0){
					cerr << "Could not create a group.\n";
					return 1;
				}
				cout << "Created group: " << *entry.title << '\n';
				for (const auto& k : *(j->key_value("children")->x.a)){
					stk.emplace(k,gid);
				}
				break;
			case DBE::Link:
				if (entry.linkValid(j)){
					entry.linkInsert(my_db,gid);
					cout << "Inserted link: ";
					abridgedOut(cout,*entry.title,36);
					cout << '\n';
				}
				break;
			default:
				break;
		}
	}
	my_db.export_memory("/mnt/ramdisk/test.db");
	cout << "End Save\n";
	return 0;
}
