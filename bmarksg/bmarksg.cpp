#include "bmarksg.h"
#include <vector>
using std::vector;

int prepare_helper(sqlite3 *db,sqlite3_stmt **s,const char* str){
	// Helps prepare statements for an database.
	// Basically, it uses the full string without a ptr
	// to remaining chars (of which they are not.)
	return sqlite3_prepare_v2(
		db,	str,	-1,	s,	0
	);
}

bool successful_stmt(sqlite3_stmt *s){
	// Given a statement (with no intended output), repeat
	// step until 'done'. Then it resets the statement.
	int R;
	do {
		R = sqlite3_step(s);
	} while (R==SQLITE_ROW);
	sqlite3_reset(s);
	return R==SQLITE_DONE;
}
DB_Connection::DB_Connection(){
	// will use :memory: in future.
	sqlite3_open("/mnt/ramdisk/db.db",&db);
	create_tables();
	prepare_stmts();
}
DB_Connection::~DB_Connection(){
	finalize_stmts();
	sqlite3_close(db);
}

void DB_Connection::create_tables(){
	// groups.gid is an alias for groups.rowid
	sqlite3_exec(
		db,
		"CREATE TABLE groups(gid INTEGER primary key,name TEXT);"
			"CREATE TABLE rel(gid INTEGER,pid INTEGER,depth INTEGER);"
			"CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);",
		0, 0, 0
	);
}
void DB_Connection::prepare_stmts(){
	// Prepares all stmts used by the connection.
	prepare_helper(db,&new_grp,
		"INSERT INTO groups (name) VALUES(?1);");
	prepare_helper(db,&self_rel,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,?1,0);");
	prepare_helper(db,&root_grp,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,NULL,1);");
	prepare_helper(db,&child_grp,
		"INSERT INTO rel SELECT ?1,pid,depth+1 FROM rel WHERE rel.gid=?2 AND rel.pid IS NOT NULL;");
	// For deleting groups
	prepare_helper(db,&del_grp,
		"DELETE FROM groups WHERE gid=?1 LIMIT 1;");
	prepare_helper(db,&del_grp_data,
		"DELETE FROM data WHERE gid=?1;");
	prepare_helper(db,&del_grp_rel,
		"DELETE FROM rel WHERE gid=?1");
	prepare_helper(db,&get_child_grp,
		"SELECT gid FROM rel WHERE pid=?1;");
	// Transaction stmts
	prepare_helper(db,&begin_trans,
		"BEGIN TRANSACTION;");
	prepare_helper(db,&end_trans,
		"END TRANSACTION;");
}
void DB_Connection::finalize_stmts(){
	// For creating groups.
	sqlite3_finalize(new_grp);
	sqlite3_finalize(self_rel);
	sqlite3_finalize(root_grp);
	sqlite3_finalize(child_grp);
	// For deleting groups.
	sqlite3_finalize(del_grp);
	sqlite3_finalize(del_grp_data);
	sqlite3_finalize(del_grp_rel);
	sqlite3_finalize(get_child_grp);
	// Transaction stmts
	sqlite3_finalize(begin_trans);
	sqlite3_finalize(end_trans);
}
int DB_Connection::create_group(const string& name,const int parent_gid){
	// Creates a group with name. If parent_gid is included, create under it.
	// Returns gid of created group.
	trans_act('B');
	sqlite3_bind_text(new_grp,1,name.c_str(),-1,SQLITE_STATIC); // STATIC is okay
	if (!successful_stmt(new_grp)){
		return 0;
	}
	int gid = sqlite3_last_insert_rowid(db);
	sqlite3_bind_int(self_rel,1,gid);
	if (!successful_stmt(self_rel)){
		return 0;
	}
	int R;
	if (parent_gid!=0){
		// has a parent // not a root group.
		sqlite3_bind_int(child_grp,1,gid);
		sqlite3_bind_int(child_grp,2,parent_gid);
		R = successful_stmt(child_grp);
	} else {
		// a root group // no parent.
		sqlite3_bind_int(root_grp,1,gid);
		R = successful_stmt(root_grp);
	}
	if (!R){
		return 0;
	}
	trans_act('E');
	return gid;
}
void DB_Connection::print_groups(){
	sqlite3_stmt *groups;
	prepare_helper(db,&groups,
		"SELECT gid,name FROM groups;");
	int R = sqlite3_step(groups);
	while (R==SQLITE_ROW){
		std::cout 
			<< sqlite3_column_int(groups,0)
			<< '|'
			<< sqlite3_column_text(groups,1)
			<< '\n';
		R = sqlite3_step(groups);
	}
	sqlite3_finalize(groups);
	std::cout << "GID|PID|DEPTH\n";
	sqlite3_stmt *relations;
	prepare_helper(db,&relations,
		"SELECT gid,pid,depth FROM rel;");
	R = sqlite3_step(relations);
	while (R==SQLITE_ROW){
		std::cout
			<< sqlite3_column_int(relations,0)
			<< '|'
			<< sqlite3_column_int(relations,1)
			<< '|'
			<< sqlite3_column_int(relations,2)
			<< '\n';
		R = sqlite3_step(relations);
	}
	sqlite3_finalize(relations);
}

bool DB_Connection::delete_group_helper(const int gid){
	// Deletes a group from:
	// the groups table,
	// its data from the data table,
	// and its relations from the rel table.
	sqlite3_bind_int(del_grp,1,gid);
	sqlite3_bind_int(del_grp_data,1,gid);
	sqlite3_bind_int(del_grp_rel,1,gid);
	if (!successful_stmt(del_grp) ||
		!successful_stmt(del_grp_data) ||
		!successful_stmt(del_grp_rel)
	){
		// If any fails, it will short-circuit and return 0.
		return 0;
	}
	return 1;
}

bool DB_Connection::delete_group(const int gid){
	// Deletes group with gid. If it doesn't exist,
	// nothing will be deleted.
	// If this function returns 0, you should rollback.
	trans_act('B');
	if (!delete_group_helper(gid)){
		return 0;
	}

	sqlite3_bind_int(get_child_grp,1,gid);
	for (
		int R = sqlite3_step(get_child_grp);
		R==SQLITE_ROW;
		R = sqlite3_step(get_child_grp)
	){
		// Loop to delete subgroups.
		if (!delete_group_helper(sqlite3_column_int(get_child_grp,0))){
			return 0;
		}
	}
	sqlite3_reset(get_child_grp);
	trans_act('E');
	return 1;
}
void DB_Connection::trans_act(const char B_E){
	sqlite3_stmt* s = ((B_E=='B') ? (begin_trans) : (end_trans));
	sqlite3_step(s);
	sqlite3_reset(s);
}
