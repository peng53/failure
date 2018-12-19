#include <sqlite3.h>
#include <string>

using std::string;

class DB_Connection {
	private:
		sqlite3 *db;
		sqlite3_stmt *new_group, *root_group, *child_group;
	public:
		DB_Connection();
		~DB_Connection();
		void create_table();
		int create_group(const string& name,int parent_gid=0);
};

DB_Connection::DB_Connection(){
	sqlite3_prepare_v2(
		db,
		"INSERT INTO groups (NULL,?1)",
		-1,
		&new_group,
		0
	);
	sqlite3_prepare_v2(
		db,
		"INSERT INTO rel VALUES(NULL,?1,?1,0);"
		"INSERT INTO rel VALUES(NULL,?1,NULL,1);",
		-1,
		&root_group,
		0
	);
	sqlite3_prepare_v2(
		db,
		"INSERT INTO rel VALUES(NULL,?1,?1,0);"
		"INSERT INTO rel SELECT NULL,?1,pid,depth+1 FROM rel"
			"WHERE rel.gid=?2 AND rel.pid IS NOT NULL;",
		-1,
		&child_group,
		0
	);
}
DB_Connection::~DB_Connection(){
	sqlite3_finalize(new_group);
	sqlite3_finalize(root_group);
	sqlite3_finalize(child_group);
	sqlite3_close(db);
}

void DB_Connection::create_table(){
	// TODO check if db is already open before.
	sqlite3_open(":memory:",&db);
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
int DB_Connection::create_group(const string& name,int parent_gid){
	// Creates a group with name. If parent_gid is included, create under it.
	// Returns gid of created group.
	sqlite3_bind_text(new_group,1,name.c_str(),-1,SQLITE_STATIC); // STATIC is okay
	int R = sqlite3_step(new_group);
	if (R!=SQLITE_DONE){
		return 0;
	}
	int gid = sqlite3_last_insert_rowid(db);
	sqlite3_reset(new_group);
	if (parent_gid!=0){
		// has a parent // not a root group.
		sqlite3_bind_int(child_group,1,gid);
		sqlite3_bind_int(child_group,2,parent_gid);
		do {
			R = sqlite3_step(child_group);
		} while (R==SQLITE_ROW);
		if (R!=SQLITE_DONE){
			return 0;
		}
		sqlite3_reset(child_group);
	} else {
		// a root group // no parent.
		sqlite3_bind_int(root_group,1,gid);
		do {
			R = sqlite3_step(root_group);
		} while (R!=SQLITE_DONE);
		if (R!=SQLITE_DONE){
			return 0;
		}
		sqlite3_reset(root_group);
	}
	return gid;
}
