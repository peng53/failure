#include "bmarksg.h"

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
	// TODO check if db is already open before.
	sqlite3_exec(
		db,
		"CREATE TABLE groups(rowid INTEGER primary key,name TEXT);"
			"CREATE TABLE rel(rowid INTEGER primary key, gid INTEGER,pid INTEGER,depth INTEGER);"
			"CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);",
		0, 0, 0
	);
}
void DB_Connection::prepare_stmts(){
	// Prepares all stmts used by the connection.
	prepare_helper(db,&new_group,
		"INSERT INTO groups (name) VALUES(?1);");
	prepare_helper(db,&self_rel,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,?1,0);");
	prepare_helper(db,&root_group,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,NULL,1);");
	prepare_helper(db,&child_group,
		"INSERT INTO rel SELECT NULL,?1,pid,depth+1 FROM rel WHERE rel.gid=?2 AND rel.pid IS NOT NULL;");
}
void DB_Connection::finalize_stmts(){
	sqlite3_finalize(new_group);
	sqlite3_finalize(self_rel);
	sqlite3_finalize(root_group);
	sqlite3_finalize(child_group);
}
int DB_Connection::create_group(const string& name,const int parent_gid){
	// Creates a group with name. If parent_gid is included, create under it.
	// Returns gid of created group.
	sqlite3_bind_text(new_group,1,name.c_str(),-1,SQLITE_STATIC); // STATIC is okay
	if (!successful_stmt(new_group)){
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
		sqlite3_bind_int(child_group,1,gid);
		sqlite3_bind_int(child_group,2,parent_gid);
		R = successful_stmt(child_group);
	} else {
		// a root group // no parent.
		sqlite3_bind_int(root_group,1,gid);
		R = successful_stmt(root_group);
	}
	if (!R){
		return 0;
	}
	return gid;
}
void DB_Connection::print_groups(){
	sqlite3_stmt *groups;
	prepare_helper(db,&groups,
		"SELECT rowid,name FROM groups;");
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
bool DB_Connection::delete_group(const int gid){
	// Deletes group with gid. If it doesn't exist,
	// nothing will be deleted.
	// These stmts will be moved later on to limit/
	// widen scope. If this function returns 0, you should rollback.
	sqlite3_exec(
		db,
		"BEGIN TRANSACTION;",
		0, 0, 0
	);
	sqlite3_stmt *delete_group, *delete_group_data, *delete_group_rel;
	prepare_helper(db,&delete_group,
		"DELETE FROM groups WHERE rowid=?1 LIMIT 1;");
	sqlite3_bind_int(delete_group,1,gid);
	if (!successful_stmt(delete_group)){
		return 0;
	}
	prepare_helper(db,&delete_group_data,
		"DELETE FROM data WHERE gid=?1;");
	sqlite3_bind_int(delete_group_data,1,gid);
	if (!successful_stmt(delete_group_data)){
		return 0;
	}

	prepare_helper(db,&delete_group_rel,
		"DELETE FROM rel WHERE gid=?1");
	sqlite3_bind_int(delete_group_rel,1,gid);
	if (!successful_stmt(delete_group_rel)){
		return 0;
	}

	sqlite3_exec(
		db,
		"END TRANSACTION;",
		0, 0, 0
	);
	sqlite3_finalize(delete_group);
	sqlite3_finalize(delete_group_data);
	sqlite3_finalize(delete_group_rel);
	return 1;
}
