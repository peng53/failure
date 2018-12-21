#ifndef BMARKSG
#define BMARKSG
#include <sqlite3.h>
#include <string>
#include <iostream>

using std::string;

class DB_Connection {
	private:
		sqlite3 *db;
		sqlite3_stmt *new_grp, *self_rel, *root_grp, *child_grp,
			*del_grp, *del_grp_data, *del_grp_rel, *get_child_grp,
			*begin_trans, *end_trans;
	public:
		DB_Connection();
		~DB_Connection();
		void create_tables();
		void prepare_stmts();
		void finalize_stmts();
		int create_group(const string& name,const int parent_gid=0);
		bool delete_group_helper(const int gid);
		bool delete_group(const int gid);
		void print_groups();
		void trans_act(const char B_E);
};
#endif
