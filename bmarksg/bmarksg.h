#ifndef BMARKSG
#define BMARKSG
#include <sqlite3.h>
#include <string>
#include <iostream>

using std::string;

class DB_Connection {
	private:
		sqlite3 *db;
		sqlite3_stmt *new_group, *self_rel, *root_group, *child_group;
	public:
		DB_Connection();
		~DB_Connection();
		void create_tables();
		void prepare_stmts();
		void finalize_stmts();
		int create_group(const string& name,const int parent_gid=0);
		bool delete_group(const int gid);
		void print_groups();
};
#endif
