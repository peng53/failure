#include "bmarksg.h"

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
		0,
		0,
		0
	);
}
void DB_Connection::prepare_stmts(){
	sqlite3_prepare_v2(
		db,
		"INSERT INTO groups (name) VALUES(?1)",
		-1,
		&new_group,
		0
	);
	sqlite3_prepare_v2(
		db,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,?1,0);",
		-1,
		&self_rel,
		0
	);
	sqlite3_prepare_v2(
		db,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,NULL,1);",
		-1,
		&root_group,
		0
	);
	int R = sqlite3_prepare_v2(
		db,
		"INSERT INTO rel SELECT NULL,?1,pid,depth+1 FROM rel WHERE rel.gid=?2 AND rel.pid IS NOT NULL;",
		-1,
		&child_group,
		0
	);
	std::cout << "child group was made: " << R << '\n';
}
void DB_Connection::finalize_stmts(){
	sqlite3_finalize(new_group);
	sqlite3_finalize(self_rel);
	sqlite3_finalize(root_group);
	sqlite3_finalize(child_group);
}
int DB_Connection::create_group(const string& name,int parent_gid){
	// Creates a group with name. If parent_gid is included, create under it.
	// Returns gid of created group.
	sqlite3_bind_text(new_group,1,name.c_str(),-1,SQLITE_STATIC); // STATIC is okay
	if (!successful_stmt(new_group)){
		return 0;
	}
	int gid = sqlite3_last_insert_rowid(db);
	int R = sqlite3_bind_int(self_rel,1,gid);
	std::cout << R;
	if (!successful_stmt(self_rel)){
		return 0;
	}
	if (parent_gid!=0){
		// has a parent // not a root group.
		std::cout << "child group\n";
		R = sqlite3_bind_int(child_group,1,gid);
		std::cout << R;
		if (R!=SQLITE_OK){ return 0; }
		R = sqlite3_bind_int(child_group,2,parent_gid);
		std::cout << R;
		R = successful_stmt(child_group);
	} else {
		// a root group // no parent.
		R = sqlite3_bind_int(root_group,1,gid);
		std::cout << R;
		R = successful_stmt(root_group);
	}
	if (!R){
		return 0;
	}
	std::cout << "OK\n";
	return gid;
}
void DB_Connection::print_groups(){
	sqlite3_stmt *groups;
	sqlite3_prepare_v2(
		db,
		"SELECT rowid,name FROM groups;",
		-1,
		&groups,
		0
	);
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
	sqlite3_prepare_v2(
		db,
		"SELECT gid,pid,depth FROM rel;",
		-1,
		&relations,
		0
	);
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
