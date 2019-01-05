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
			*begin_trans, *end_trans,
			*set_grp_name, *childNparent, *ins_data,
			*del_data, *attach_output, *copy_to_m;
		void create_tables();
		void prepare_stmts();
		void finalize_stmts();
	public:
		DB_Connection();
		~DB_Connection();
		bool child_link(const int root,const int child);
		bool unlink(const int gid);
		int create_group(const string& name,const int parent_gid=0);
		bool delete_group_helper(const int gid);
		bool delete_group(const int gid);
		void print_groups();
		void trans_act(const char B_E);
		bool rename_group(const int gid,const string& s);
		bool change_parent(const int gid,const int new_par=0);
		bool add_data(const string& key,const string& value,const string& mtime,const int gid=0);
		bool remove_data(const int rowid);
		bool export_memory(const string& filename);
};
#endif
